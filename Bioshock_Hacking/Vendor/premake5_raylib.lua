project "Raylib"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    systemversion "latest"
    cdialect "C99"

    targetdir "Raylib/lib/%{cfg.buildcfg}"
    objdir "Raylib/lib/%{cfg.buildcfg}"

    files {
        "Raylib/src/rcore.c",
        "Raylib/src/raudio.c",
        "Raylib/src/rmodels.c",
        "Raylib/src/rshapes.c",
        "Raylib/src/rtext.c",
        "Raylib/src/rtextures.c",
        "Raylib/src/utils.c",

        "Raylib/src/raymath.h",
        "Raylib/src/rlgl.h"
    }

    defines {
        "PLATFORM_DESKTOP_RGFW",
        "GRAPHICS_API_OPENGL_46"
    }

    -- Special case for visual studio
    filter "action:vs*"
		warnings "Off" -- Options: Off, Extra, High, Default
        defines {
            "_CRT_SECURE_NO_WARNINGS" 
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


-- Cleaning files specific for this project
if _ACTION == 'clean' then
	print("Raylib: Cleaning up library files")
	os.rmdir("Raylib/lib")
end	