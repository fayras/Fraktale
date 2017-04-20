cmake_minimum_required(VERSION 3.5)
project(Fraktale)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
    set(CMAKE_CXX_COMPILER "clang++")
  endif()
endif()

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 14)
endif ()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Set path to Qt Libraries.
set (CMAKE_PREFIX_PATH "../Qt/5.8/gcc_64/lib/cmake")

# Find Qt Libraries.
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

set(SOURCE_FILES main.cpp FractalWindow.cpp FractalWindow.hpp Fractals/Fractal.cpp Fractals/Fractal.hpp Fractals/Mandelbrot.cpp Fractals/Mandelbrot.hpp Canvas.cpp Canvas.hpp Rendering/MandelbrotRenderTask.cpp Rendering/MandelbrotRenderTask.hpp Fractals/EmptyFractal.cpp Fractals/EmptyFractal.hpp ColorModes/ColorMode.cpp ColorModes/ColorMode.hpp ColorModes/WaveLengthMode.cpp ColorModes/WaveLengthMode.hpp Fractals/Identifiers.hpp ColorModes/Grayscale.cpp ColorModes/Grayscale.hpp ColorModes/Identifiers.hpp ColorModes/SmoothGradient.cpp ColorModes/SmoothGradient.hpp Fractals/FractalPixelIteration.hpp)
add_executable(Fraktale ${SOURCE_FILES})

# Use the modules from Qt 5.
target_link_libraries(Fraktale Qt5::Widgets Qt5::Core Qt5::Gui)