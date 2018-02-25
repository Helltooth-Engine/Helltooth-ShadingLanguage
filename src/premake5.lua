function setProjectOutputDirectories() 
	objdir "$(SolutionDir)\\Intermediates\\"
	targetdir "$(SolutionDir)\\bin\\$(Platform)\\$(Configuration)\\$(ProjectName)\\"
end


function setConfigurationFilters() 
	filter {"configurations:Debug-GL"}
		defines {
			"HT_OPENGL",
			"HT_DEBUG"
		}
	filter {"configurations:Release-GL"}
		defines {
			"HT_OPENGL",
			"HT_RELEASE"
		}
		optimize "Full"
   		symbols "Off"
	filter {"configurations:Debug-DX"}
		defines {
			"HT_DIRECTX",
			"HT_DEBUG"
		}
	filter {"configurations:Release-DX"}
		defines {
			"HT_DIRECTX",
			"HT_RELEASE"
		}
		optimize "Full"
   		symbols "Off"

	filter {"platforms:x86"}
		defines {
			"HT_WINDOWS"
		}
	filter {"platforms:x64"}
		defines {
			"HT_WINDOWS"
		}
	filter {}
end


workspace ('Helltooth-ShadingLanguage')
	configurations {
		"Debug-GL",
		"Release-GL",
		"Debug-DX",
		"Release-DX",
	}

	platforms {
		"x86",
		"x64",
	}

	location "../Solution/"
	startproject "Sandbox"


project ("Helltooth-ShadingLanguage")
	kind ("StaticLib")
	files {
		"htsl/**.hpp",
		"htsl/**.cpp",
	}

	location "../Solution/Helltooth-ShadingLanguage/"
	includedirs {
		"$(SolutionDir)../src/htsl/"
	}

	setProjectOutputDirectories() 
    setConfigurationFilters()

project ("Sandbox")
	kind ("ConsoleApp")
	files {
		"demo/**.hpp",
		"demo/**.cpp",
		"../Solution/Sandbox/res/**.*",
	}
	location "../Solution/Sandbox"
	dependson "Helltooth-ShadingLanguage"

	links {
		"Helltooth-ShadingLanguage.lib"
	}

	sysincludedirs {
		"$(SolutionDir)../src/htsl/"
	}

	syslibdirs {
		"$(SolutionDir)bin\\$(Platform)\\$(Configuration)\\Helltooth-ShadingLanguage\\"
	}

	setProjectOutputDirectories() 
    setConfigurationFilters()