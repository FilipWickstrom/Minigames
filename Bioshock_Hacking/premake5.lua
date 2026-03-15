project "Bioshock_Hacking"
	kind			"ConsoleApp"
	language		"C++"
	staticruntime	"on"
	systemversion	"latest"
	cppdialect      "C++23"

	targetdir("Build/Bin/" .. outputdir)
	objdir("Build/Bin-int/" .. outputdir)

	defines { 'PROJECT_DIR="' .. os.getcwd():gsub("\\", "/") .. '"' }

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs 
	{
		"../Vendor/Raylib/src",
		"../Vendor/rlImGui",
		"../Vendor/ImGui"
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