workspace "Raiden Shogun"
	architecture "x64"
	configurations { "Release" }

project "Raiden Shogun"
	kind          "ConsoleApp"
	language      "C++"
	cppdialect    "C++17"
	staticruntime "off"

	outputdir = "%{cfg.buildcfg}"

	targetdir ("%{wks.location}/x64/%{cfg.buildcfg}")
	objdir    ("%{wks.location}/x64/%{cfg.buildcfg}")

	includedirs{ "include" }
	libdirs    { "lib" }
	links      { "dpp.lib" }
	files      { 
		"src/**.cpp", 
		"src/moderation/**.cpp", 
		"src/utility/**.cpp",
		"commands/**.h", 
		"commands/moderation/**.h",
		"commands/utility/**.h",
		"Raiden Shogun.rc" 
	}

	filter { "system:windows", "toolset:msc" }
		systemversion "latest"

	filter "configurations:Release"
		runtime  "Release"
		optimize "on"
