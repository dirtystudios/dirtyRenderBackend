project "RenderBackend"
    kind "StaticLib"
    language "C++"
    targetdir "lib"

    files {
        "src/**.cpp",
        "src/**.h",
    }

    filter "configurations:release"
        flags { "LinkTimeOptimization" }

    filter "system:windows"
        characterset "MBCS" -- oops

        libdirs { "external/winlibs/%{cfg.platform}" }
        links { 
            "d3d11",
            "d3dcompiler",
            "DXGI",
            "dxguid",
        }

    filter "system:macosx"
        buildoptions { "-x objective-c++"}
        links { 
            "Metal.framework",
            "QuartzCore.framework",
            "Cocoa.framework",
        }
        files { "src/**.mm" }
        removefiles { "src/**/dx11/**"}