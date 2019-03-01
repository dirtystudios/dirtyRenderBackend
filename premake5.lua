workspace "RenderBackend"
    location "build"
    configurations { "debug", "release" }
    platforms { "x32", "x64" }

    vectorextensions "SSE2"

    filter "platforms:x32"
        architecture "x86"
    filter "platforms:x64"
        architecture "x86_64"

    filter "configurations:debug"
        runtime "Debug"
        optimize "Debug"
        symbols "On"
        targetsuffix "d"
        defines { 
            "_DEBUG"
        }
    filter "configurations:release"
        runtime "Release"
        optimize "On"
		symbols "On"
        defines { "NDEBUG" }

    filter "system:windows"
        defines { "_WINDOWS", "WIN32" }
    -- optimization levels in debug running debug stepping
    filter { "configurations:debug", "system:macosx" }
        xcodebuildsettings {['GCC_OPTIMIZATION_LEVEL'] = 0}
    filter {}

    cppdialect "C++17"
    xcodebuildsettings {['CLANG_CXX_LANGUAGE_STANDARD'] = 'c++17'}


    include "gfx.lua"
    -- vpaths { 
    --     -- move all src files up one 
    --     ["*"] = "src",
    -- }
