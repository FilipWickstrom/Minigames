-- Download ImGui if directory not found
function check_imgui()
	os.chdir("rlImGui")
	if(os.isdir("imgui-master") == false) then
		if(not os.isfile("imgui-master.zip")) then
			print("imgui not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/ocornut/imgui/archive/refs/heads/master.zip", "imgui-master.zip", {
				null,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("imgui-master.zip", os.getcwd())
		os.remove("imgui-master.zip")
	end

	os.chdir("..")
end

check_imgui()

project "rlImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"
	
	targetdir "rlImGui/lib/%{cfg.buildcfg}"
    objdir "rlImGui/lib/%{cfg.buildcfg}"
	
	includedirs { 
		"Raylib/src",
		"rlImGui/imgui-master"
	}

	files {
        "rlImGui/rlImGui.cpp",
		"rlImGui/rlImGui.h",
		"rlImGui/rlImGuiColors.h",
		"rlImGui/imgui-master/*.h",
		"rlImGui/imgui-master/*.cpp"
    }

	defines {"IMGUI_DISABLE_OBSOLETE_FUNCTIONS","IMGUI_DISABLE_OBSOLETE_KEYIO"}

