workspace "Minigames"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release"
	}
	startproject "Bioshock_Hacking" --TODO Fix this somehow?

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Vendor"
	include "Vendor/premake5_raylib"
	include "Vendor/premake5_raylibImGui"
group ""

group "Games"
	include "DyingLight_Lockpicking/premake5"
	include "Bioshock_Hacking/premake5"
	-- More projectes?
group ""



-- Cleaning up the project
newaction 
{
	trigger = "clean",
	description = "onCleanProject",
	execute = function()
		print("Removing Visual Studio files")
		os.rmdir(".vs")
		os.remove("**sln")
		os.remove("**vcxproj.user")
		os.remove("**vcxproj.filters")
		os.remove("**.vcxproj")
			
		print("Removing makefiles")
		os.remove("**Makefile")
		os.remove("**.make")

		print("Cleaning up raylib")
		os.rmdir("Vendor/Raylib/lib")

		print("Cleaning up rlImGui")
		os.rmdir("Vendor/rlImGui/lib")
		
		print("Removing build from games")
		os.rmdir("DyingLight_Lockpicking/Build")
		os.rmdir("Bioshock_Hacking/Build")
	end
}

