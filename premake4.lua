
-- A solution contains projects, and defines the available configurations
solution "CUtils"
	configurations { "Debug", "Release" }
	location "build"
 

	-- A project defines one build target
	project "driver"
		location "build"
		kind "ConsoleApp"
		language "C"
		files
		{
			"main.c",
			"c_utils.c",
			"c_utils.h",
			"rsw_cstr.c",
			"rsw_cstr.h",
			"basic_types.h",
			"priority_queue.h"
		}
--		excludes { "c_utils.*" }
--		libdirs { "/usr/lib64/" }
--		includedirs { "./inc" }
		links { "cunit" } 
		targetdir "build"

		configuration "Debug"
			defines { "DEBUG" }
			symbols "On"

		configuration "Release"
			defines { "NDEBUG" }
			symbols "Off"
			optimize "On"

		configuration { "linux", "gmake" }
			buildoptions { "-std=c99", "-pedantic" }


	project "cppdriver"
		location "build"
		kind "ConsoleApp"
		language "C++"
		files
		{
			"main.cpp",
			"c_utils.cpp",
			"c_utils.h",
			"rsw_cstr.cpp",
			"rsw_cstr.h",
			"basic_types.h"
		}
 --   excludes { "c_utils.*" }
 --   libdirs { "/usr/lib64/" }
 --   includedirs { "./inc" }
 --   links { "c_utils" } 
	  targetdir "build"
  
		configuration "Debug"
			defines { "DEBUG" }
			symbols "On"
 
		configuration "Release"
			defines { "NDEBUG" }
			symbols "Off"
			optimize "On"
 
		configuration { "linux", "gmake" }
		buildoptions { "-ansi", "-pedantic-errors", "-fno-rtti", "-fno-exceptions", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }
-- "-x c++" //force compiling as C++, ignore extension
--[[
	project "c_utils"
		kind "SharedLib"
		language "C"
		files { "./src/**.c" }
		excludes { "./src/main.c" }
		includedirs { "./inc" }

	
	project "c_utils"
		kind "StaticLib"
		language "C"
		files { "./src/**.c" }
		excludes { "./src/main.c" }
		includedirs { "./inc" }
		]]
