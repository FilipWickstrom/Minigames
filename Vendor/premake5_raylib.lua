project "Raylib"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    systemversion "latest"
    cdialect "C99"

    targetdir "Raylib/lib/%{cfg.buildcfg}"
    objdir "Raylib/lib/%{cfg.buildcfg}"
	
    files 
	{
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

    defines 
	{
       "PLATFORM_DESKTOP_RGFW",
       "GRAPHICS_API_OPENGL_46"
    }

    -- Special case for visual studio
    filter "action:vs*"
		warnings "Off" -- Options: Off, Extra, High, Default
        defines 
		{
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "_RELEASE"
		optimize "On"
