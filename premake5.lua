function setProjectOutputDirectories() 
	objdir "$(SolutionDir)\\Intermediates\\"
	targetdir "$(SolutionDir)\\bin\\$(Platform)\\$(Configuration)\\$(ProjectName)\\"
end


function setConfigurationFilters() 
	filter {"configurations:OpenGL-Debug"}
		defines {
			"HT_OPENGL",
			"HT_DEBUG"
		}
	filter {"configurations:OpenGL-Release"}
		defines {
			"HT_OPENGL",
			"HT_RELEASE"
		}
	filter {"configurations:DirectX-Debug"}
		defines {
			"HT_DIRECTX",
			"HT_DEBUG"
		}
	filter {"configurations:DirectX-Release"}
		defines {
			"HT_DIRECTX",
			"HT_RELEASE"
		}

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
		"OpenGL-Debug",
		"OpenGL-Release",
		"DirectX-Debug",
		"DirectX-Release",
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
		"$(SolutionDir)..src/htsl/"
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