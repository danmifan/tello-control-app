workspace "tello-control-app"
  configurations { "Debug", "Release" }
  -- configurations {"Debug"}

-- include("conanbuildinfo.premake.lua")

  buildoptions {"--std=c++14"}

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"


-- linkoptions { }

project "imgui"
  kind "SharedLib"
  language "C++"
  targetdir "lib/"

  includedirs {
    "thirdparty/imgui"
  }

  links {
    "glfw"
  }

  files {
    "thirdparty/imgui/*.cpp",
    "thirdparty/imgui/backends/imgui_impl_glfw.cpp",
    "thirdparty/imgui/backends/imgui_impl_opengl3.cpp",
  }

-- project "dji-control-app"
--   kind "SharedLib"
--   language "C++"
--   targetdir "lib/"

--   includedirs {
--     "include",
--     "thirdparty/imgui",
--     "thirdparty/imgui-sfml"
--   }

--   libdirs {
--     "lib",
--   }

--   links {
--   }

--   files {
--     "src/engine.cpp",
--     "src/entity.cpp",
--     "src/gui/*.cpp"
--   }

project "tello-control-app"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/"

  includedirs {
    "include",
    "thirdparty/imgui",
    "thirdparty/imgui/backends"
  }

  libdirs {
    "lib",
  }

  links {
    "imgui",
    "glfw",
    "GL"
  }

  files {
    "src/main.cpp"
  }
