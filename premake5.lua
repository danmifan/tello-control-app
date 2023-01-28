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

project "window"
  kind "SharedLib"
  language "C++"
  targetdir "lib/"

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
    "src/window/window.cpp"
  }

project "tello-control-app"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/"

  includedirs {
    "include",
  }

  libdirs {
    "lib",
  }

  links {
    "imgui",
    "window",
    "pthread"
  }

  files {
    "src/*.cpp"
  }
