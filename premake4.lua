
-- A solution contains projects, and defines the available configurations
solution "CUtils"
   configurations { "Debug", "Release" }
   location "build"
 

   -- A project defines one build target
   project "driver"
      location "build"
      kind "ConsoleApp"
      language "C"
      files { "*.c" }
 --   excludes { "c_utils.*" }
 --   libdirs { "/usr/lib64/" }
 --   includedirs { "./inc" }
 --   links { "c_utils" } 
	  targetdir "build"
  
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
 
      configuration { "linux", "gmake" }
         buildoptions { "-std=c99", "-pedantic" }
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
