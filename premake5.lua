workspace "tello-control-app"
  configurations { "Debug", "Release" }

  buildoptions {"--std=c++14", "-Wall", "-Wextra"}

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"


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
    "src/window/*.cpp"
  }

project "tello-control-app"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/"

  -- buildoptions { "`pkg-config --cflags opencv4`"}
  -- linkoptions {"`pkg-config --libs opencv4`" }

  includedirs {
    "include",
    "thirdparty/imgui",
    "/usr/local/include/opencv4"
  }

  libdirs {
    "lib",
    "/usr/local/lib"
  }

  links {
    "imgui",
    "window",
    "pthread",
    "opencv_core",
    "opencv_videoio"
  }

  files {
    "src/*.cpp",
    "src/views/*.cpp"
  }

-- project "cli"
--   kind "ConsoleApp"
--   language "C++"
--   targetdir "bin/"

--   files {
--     "src/cli/cli.cpp"
--   }
