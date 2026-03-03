workspace "GameJamTemplate"
	architecture "x64"
	startproject "Game"
	
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vendor/premake5_raylib.lua"
	include "Vendor/premake5_raylibImGui.lua"
group ""

project "Game"
	location		"Game"
	kind			"ConsoleApp"
	language		"C++"
	staticruntime	"on"
	systemversion	"latest"
	cppdialect      "C++23"

	targetdir("Build/Bin/" .. outputdir .. "/%{prj.name}")
	objdir("Build/Bin-int/" .. outputdir .. "/%{prj.name}")
	defines { 'PROJECT_DIR="' .. os.getcwd():gsub("\\", "/") .. '"' }

	files
	{
		"Game/**.h",
		"Game/**.cpp"
	}

	includedirs 
	{
		"Vendor/Raylib/src",
		"Vendor/rlImGui",
		"Vendor/rlImGui/imgui-master"
	}
	
	links
	{
		"Raylib",
		"rlImGui",
	}

	-- Ugly solution...
	filter "system:windows"
		linkoptions { "/FORCE:MULTIPLE" }

	filter "system:linux"
		links 
		{
			"X11",
			"GL",
			"GLU",
			"pthread",
			"dl",
			"m",
			"Xrandr"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "_RELEASE"
		optimize "On"
				
-- Cleaning up the project
newaction {
	trigger = "clean",
	description = "onCleanProject",
	execute = function()
		print("Removing build files")
		os.rmdir("Build")
		
		print("Removing Visual Studio files")
		os.rmdir(".vs")
		os.remove("*.sln")
		os.remove("Game/Game.vcxproj")
		os.remove("Game/Game.vcxproj.user")
		os.remove("Vendor/*.vcxproj")
			
		print("Removing makefiles")
		os.remove("**Makefile")
		os.remove("**.make")

		print("Cleaning up raylib")
		os.rmdir("Vendor/Raylib/lib")

		print("Cleaning up rlImGui")
		os.rmdir("Vendor/rlImGui/lib")
	end
	
	
}