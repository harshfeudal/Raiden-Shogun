workspace "Raiden Shogun"
	architecture "x64"

	configurations
	{
		"Release"
	}

project "Raiden Shogun"
	kind          "ConsoleApp"
	language      "C++"
	cppdialect    "C++17"
	staticruntime "off"

    -- Working in progress --

	outputdir = "%{cfg.buildcfg}"

	targetdir ("%{wks.location}/x64/%{cfg.buildcfg}")
	objdir    ("%{wks.location}/x64/%{cfg.buildcfg}")

	includedirs
	{
		"include"
	}

	libdirs
	{
		"lib"
	}

	links
	{
		"dpp.lib"
	}

	files
	{
		"src/**.cpp",
        "commands/**.h"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Release"
		runtime  "Release"
		optimize "on"
	
	filter("toolset:msc")
		linkoptions("/FORCE:MULTIPLE")
        