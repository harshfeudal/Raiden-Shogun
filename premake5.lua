project "Raiden Shogun"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

    -- Working in progress --

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.cpp",
        "commands/**.h"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        