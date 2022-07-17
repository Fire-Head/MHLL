
local function addSrcFiles( prefix )
	return prefix .. "/*cpp", prefix .. "/*.h"
end


workspace "MHLL"
	configurations { "Release" }
	location "build"

	files { addSrcFiles("src") }
	files { addSrcFiles("src/manhunt") }
	
	includedirs { "src" }
	includedirs { "src/manhunt" }
	includedirs { "inc" }

project "MHLL"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".asi"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK36") }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
