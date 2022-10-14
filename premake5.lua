workspace "Raiden Shogun"
	architecture "x64"

	configurations
	{
		"Release"
	}

project "Raiden Shogun"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

    -- Working in progress --

	outputdir = "%{cfg.architecture}/%{cfg.buildcfg}"

	targetdir ("%{wks.location}/" .. outputdir .. "/%{prj.name}")

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
        