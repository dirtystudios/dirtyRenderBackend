#pragma once

#include "ShaderData.h"
#include "ShaderFunctionDesc.h"
#include <vector>

namespace gfx {
    struct ShaderDataDesc
    {
        std::vector<ShaderFunctionDesc> functions;
        ShaderData data;
    };
}
