cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)

# Get folder name
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${FOLDER_NAME})

# Add executable
file(GLOB_RECURSE CPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
# Print all source files
foreach(SOURCE_FILE ${CPP_SOURCES})
    message(STATUS "Source file: ${SOURCE_FILE}")
endforeach()
add_executable(${PROJECT_NAME} ${CPP_SOURCES})

set(RAYLIB_INCLUDE_DIR "./vendor/raylib/src")
set(RAYLIB_LIB_DIR "./vendor/raylib/src")

# Includes raylib
include_directories(${RAYLIB_INCLUDE_DIR})

# Link raylib library
target_link_directories(${PROJECT_NAME} PUBLIC ${RAYLIB_LIB_DIR})
target_link_libraries(${PROJECT_NAME} raylib)

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-narrowing)
