project "rlImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"
	
	targetdir "rlImGui/lib/%{cfg.buildcfg}"
    objdir "rlImGui/lib/%{cfg.buildcfg}"
	
	includedirs 
	{ 
		"Raylib/src",
		"ImGui"
	}

	files 
	{
        "rlImGui/rlImGui.cpp",
		"rlImGui/rlImGui.h",
		"rlImGui/rlImGuiColors.h",
		"ImGui/*.h",
		"ImGui/*.cpp"
    }

	defines 
	{
		"IMGUI_DISABLE_OBSOLETE_FUNCTIONS",
		"IMGUI_DISABLE_OBSOLETE_KEYIO"
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "_RELEASE"
		optimize "On"

