workspace "BioshockHackingGame"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}

startproject "BioshockHackingGame"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vendor/premake5_raylib"
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


	files
	{
		"Game/**.h",
		"Game/**.cpp"
	}

	includedirs 
	{
		"Vendor/Raylib/src"
		--"Vendor/Raylib/src/external"
	}
	
	links
	{
		"Raylib"
	}

	filter "system:linux"
		links {
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
	trigger		= "clean",
	description = "onCleanProject",
	execute     = function()
		print("Removing build files")
		os.rmdir("Build")
		
		print("Removing Visual Studio files")
		os.rmdir(".vs")
		os.remove("*.sln")
		os.remove("Game/*.vcxproj")
		os.remove("Game/*.vcxproj.filters")
		os.remove("Vendor/*.vcxproj")
		os.remove("Vendor/*.vcxproj.filters")
		os.remove("**.vcxproj.user")
			
		print("Removing makefile")
		os.remove("**Makefile")
	end
	
	
}