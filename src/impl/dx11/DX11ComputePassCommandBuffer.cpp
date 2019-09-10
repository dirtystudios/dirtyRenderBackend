#include "DX11ComputePassCommandBuffer.h"
#include "ResourceManager.h"

#include <wrl.h>

namespace gfx {
    using namespace Microsoft::WRL;

    DX11ComputePassCommandBuffer::DX11ComputePassCommandBuffer(ComPtr<ID3D11Device> dev, ComPtr<ID3D11DeviceContext> ctx, ResourceManager* rm, DX11Cache* cache)
        : _dev(dev), _rm(rm), _cache(cache) {
        _cmdBuf.reset(new DX11Context(ctx));
    }

    void DX11ComputePassCommandBuffer::setPipelineState(PipelineStateId pipelineState) {
        PipelineStateDX11* state = _rm->GetResource<PipelineStateDX11>(pipelineState);
        _cmdBuf->SetComputeShader(state->computeShader);
    }

    void DX11ComputePassCommandBuffer::setBuffer(BufferId buffer, uint8_t index, ShaderBindingFlags bindingflags) {
        BufferDX11* cbuffer = _rm->GetResource<BufferDX11>(buffer);
        dg_assert_nm(cbuffer != nullptr);

        D3D11_BUFFER_DESC desc;
        cbuffer->buffer->GetDesc(&desc);

        if (desc.BindFlags == D3D11_BIND_CONSTANT_BUFFER) {
            BufferDX11* cbuffer = _rm->GetResource<BufferDX11>(buffer);
            _cmdBuf->SetComputeCBuffer(index, cbuffer->buffer.Get());
        }
        else if ((desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0) {
            if (bindingflags == ShaderBindingFlags::ReadOnly)
                _cmdBuf->SetComputeShaderTexture(index, cbuffer->srv.Get(), nullptr);
            else if (bindingflags == ShaderBindingFlags::ReadWrite)
                _cmdBuf->SetComputeShaderUAV(index, cbuffer->uav.Get());
            else dg_assert_fail_nm();
        }
        else {
            dg_assert_fail_nm();
        }
    }

    void DX11ComputePassCommandBuffer::setTexture(TextureId buffer, uint8_t index, ShaderBindingFlags bindingflags) {
        auto* tex = _rm->GetResource<TextureDX11>(buffer);
        if (bindingflags == ShaderBindingFlags::SampleRead)
            _cmdBuf->SetComputeShaderTexture(index, tex->srv.Get(), tex->sampler.Get());
        else if (bindingflags == ShaderBindingFlags::ReadOnly)
            _cmdBuf->SetComputeShaderTexture(index, tex->srv.Get(), nullptr);
        else if (bindingflags == ShaderBindingFlags::ReadWrite)
            _cmdBuf->SetComputeShaderUAV(index, tex->uav.Get());
    }

    void DX11ComputePassCommandBuffer::dispatch(std::array<size_t, 3> threadsPerGrid, std::array<size_t, 3> threadsPerGroup) {
        _cmdBuf->Dispatch(threadsPerGroup[0], threadsPerGroup[1], threadsPerGroup[2]);
    }
}