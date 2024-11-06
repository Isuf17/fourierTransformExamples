cmake_minimum_required(VERSION 3.10.0)
project(tutorial2 VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable generation of compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV.")
endif()
message(STATUS "Found OpenCV version ${OpenCV_VERSION}")

# Add executable
add_executable(fourierTransformExamples main.cpp)

# Include OpenCV directories and link libraries using modern CMake
target_include_directories(fourierTransformExamples PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(fourierTransformExamples PRIVATE ${OpenCV_LIBS})
