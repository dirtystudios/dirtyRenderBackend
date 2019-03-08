#include "DX11RenderPassCommandBuffer.h"
#include "SemanticNameCache.h"
#include "ResourceManager.h"

#include <wrl.h>

namespace gfx {
    using namespace Microsoft::WRL;

	static std::string VertexAttributeUsageToString(VertexAttributeUsage usage) {
		switch (usage) {
		case VertexAttributeUsage::Position:     return "POSITION";
		case VertexAttributeUsage::Normal:       return "NORMAL";
		case VertexAttributeUsage::Color0:       return "COLOR";
		case VertexAttributeUsage::Texcoord0:    return "TEXCOORD";
		case VertexAttributeUsage::BlendIndices: return "BLENDINDICES";
		case VertexAttributeUsage::BlendWeights: return "BLENDWEIGHTS";
		default: assert(false);
		}
		return "";
	}

    DX11RenderPassCommandBuffer::DX11RenderPassCommandBuffer(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> ctx, ResourceManager* rm, DX11Cache* cache)
        : _dev(dev), _rm(rm), _cache(cache) {
        _cmdBuf.reset(new DX11Context(ctx));
    }
    
    ID3D11InputLayout* DX11RenderPassCommandBuffer::CreateInputLayout(InputLayoutDX11* state, ShaderId shaderId) {
        bool first = true;
        std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;
        uint32_t stride = 0;
        for (auto layout : state->layoutDesc.elements) {
            D3D11_INPUT_ELEMENT_DESC ied;
            ied.SemanticName = SemanticNameCache::AddGetSemanticNameToCache(VertexAttributeUsageToString(layout.usage).c_str());
            ied.SemanticIndex = 0;
            ied.InputSlot = 0;
            ied.AlignedByteOffset = first ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
            ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            ied.InstanceDataStepRate = 0;
            ied.Format = GetVertexAttributeFormatDX11(layout.type, layout.storage);
            stride += GetByteCount(layout);
            first = false;
            ieds.emplace_back(ied);
        }

        ShaderDX11* shader = _rm->GetResource<ShaderDX11>(shaderId);

        ComPtr<ID3D11InputLayout> inputLayout;

        // quick verify cause why not
        HRESULT hr = _dev->CreateInputLayout(ieds.data(), static_cast<UINT>(ieds.size()),
            shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), NULL);

        if (hr == S_FALSE) {
            hr = _dev->CreateInputLayout(ieds.data(), static_cast<UINT>(ieds.size()),
                shader->blob->GetBufferPointer(), shader->blob->GetBufferSize(), &inputLayout);
            assert(hr == 0);
        }

        assert(inputLayout.Get() != 0);

        state->inputLayout.Swap(inputLayout);
        state->stride = stride;

        return state->inputLayout.Get();
    }

    void DX11RenderPassCommandBuffer::SetViewPort(uint32_t height, uint32_t width) {
        D3D11_VIEWPORT vp;
        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        _cmdBuf->SetViewport(vp);
    }

    void DX11RenderPassCommandBuffer::SetupRenderTargets(const FrameBuffer& frameBuffer, const RenderPassDX11& rp) {

		bool first = true;
		std::array<ID3D11RenderTargetView*, kMaxFramebufferColorAttachments> rtvs{};

		for (const ColorAttachmentDesc& attachment : rp.info.colorAttachments()) {
			dg_assert_nm(attachment.index < kMaxFramebufferColorAttachments && frameBuffer.color(attachment.index) != NULL_ID);
			// set viewport based on first color
			if (first) {
				first = false;
				TextureDX11* buf = _rm->GetResource<TextureDX11>(frameBuffer.color(attachment.index));
				D3D11_TEXTURE2D_DESC desc;
				buf->texture->GetDesc(&desc);
				SetViewPort(desc.Height, desc.Width);
			}
			TextureDX11* buf = _rm->GetResource<TextureDX11>(frameBuffer.color(attachment.index));
			dg_assert_nm(buf->rtv.Get() != nullptr);
			rtvs[(size_t)attachment.index] = buf->rtv.Get();
		}

		ID3D11DepthStencilView *dsv{ nullptr };
		if (rp.info.hasDepth()) {
			// set viewport on depth if no color attachment
			if (rp.info.colorAttachments().size() == 0) {
				TextureDX11* buf = _rm->GetResource<TextureDX11>(frameBuffer.depth());
				D3D11_TEXTURE2D_DESC desc;
				buf->texture->GetDesc(&desc);
				SetViewPort(desc.Height, desc.Width);
			}

			TextureDX11* buf = _rm->GetResource<TextureDX11>(frameBuffer.depth());
			dg_assert_nm(buf->dsv.Get() != nullptr);
			dsv = buf->dsv.Get();
		}
        _cmdBuf->SetRenderTargets(rtvs, rp.info.colorAttachments().size(), dsv);

        // run through and clear the needful

        for (uint32_t i = 0; i < rp.info.colorAttachments().size(); ++i) {
            const auto& a = rp.info.colorAttachments()[i];
            if (a.loadAction == LoadAction::Clear)
                _cmdBuf->ClearRenderTargetView(rtvs[i], 0.f, 0.f, 0.f, 0.f);
        }

        if (rp.info.hasDepth() || rp.info.hasStencil()) {
            TextureDX11* buf = _rm->GetResource<TextureDX11>(frameBuffer.depth());
            bool clearDepth = rp.info.hasDepth() && rp.info.depthAttachment().value().loadAction == LoadAction::Clear;
            bool clearStencil = rp.info.hasStencil() && rp.info.stencilAttachment().value().loadAction == LoadAction::Clear;
            _cmdBuf->ClearDepthStencil(buf->dsv.Get(), clearDepth, 1.f, clearStencil, 0);
        }
    }

    void DX11RenderPassCommandBuffer::setPipelineState(PipelineStateId psId) {
        PipelineStateDX11* state = _rm->GetResource<PipelineStateDX11>(psId);

        _cmdBuf->SetPrimitiveType(state->topology);

        _cmdBuf->SetDepthState(state->depthState);
        _cmdBuf->SetRasterState(state->rasterState);
        _cmdBuf->SetBlendState(state->blendState);

        _cmdBuf->SetVertexShader(state->vertexShader);
        _cmdBuf->SetPixelShader(state->pixelShader);

        // If we have an invalid layouthandle, check if we have created it yet, otherwise create it based on the given shader
        if (state->vertexLayout == nullptr) {
            dg_assert_nm(state->vertexLayoutHandle != 0);
            auto check = _rm->GetResource<InputLayoutDX11>(state->vertexLayoutHandle);
            if (check->inputLayout != 0) {
                state->vertexLayout = check->inputLayout.Get();
                state->vertexLayoutStride = check->stride;
            }
            else {
                state->vertexLayout = CreateInputLayout(check, state->vertexShaderHandle);
                state->vertexLayoutStride = check->stride;
            }
        }

        _cmdBuf->SetInputLayout(state->vertexLayoutStride, state->vertexLayout);
    }

    void DX11RenderPassCommandBuffer::setVertexBuffer(BufferId vertexBuffer) {
        auto vb = _rm->GetResource<BufferDX11>(vertexBuffer);
        _cmdBuf->SetVertexBuffer(vb->buffer.Get());
    }

    void DX11RenderPassCommandBuffer::setShaderBuffer(BufferId buffer, uint8_t index, const ShaderStageFlags& stages) {
        BufferDX11* cbuffer = _rm->GetResource<BufferDX11>(buffer);
        if (stages.test(ShaderStageBitPositionVertex))
            _cmdBuf->SetVertexCBuffer(index, cbuffer->buffer.Get());
        if (stages.test(ShaderStageBitPositionPixel))
            _cmdBuf->SetPixelCBuffer(index, cbuffer->buffer.Get());
    }

    void DX11RenderPassCommandBuffer::setShaderTexture(TextureId texture, uint8_t index, const ShaderStageFlags& stages) {
        TextureDX11* texturedx11 = _rm->GetResource<TextureDX11>(texture);
        if (stages.test(ShaderStageBitPositionVertex)) {
            dg_assert_nm(texturedx11->srv);
            _cmdBuf->SetVertexShaderTexture(index, texturedx11->srv.Get(), texturedx11->sampler.Get());
        }
        if (stages.test(ShaderStageBitPositionPixel)) {
            dg_assert_nm(texturedx11->srv);
            _cmdBuf->SetPixelShaderTexture(index, texturedx11->srv.Get(), texturedx11->sampler.Get());
        }
    }

    void DX11RenderPassCommandBuffer::drawPrimitives(uint32_t startOffset, uint32_t vertexCount) {
        _cmdBuf->DrawPrimitive(startOffset, vertexCount, false);
    }

    void DX11RenderPassCommandBuffer::drawIndexed(BufferId indexBufferId, uint32_t indexCount, uint32_t indexOffset, uint32_t baseVertexOffset) {
        auto indexBuffer = _rm->GetResource<BufferDX11>(indexBufferId);
        _cmdBuf->SetIndexBuffer(indexBuffer->buffer.Get());
        _cmdBuf->DrawPrimitive(indexOffset, indexCount, true, baseVertexOffset);
    }
}