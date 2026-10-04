cmake_minimum_required(VERSION 3.10)
project(mr-angry-cube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define raylib paths in vendor directory
set(RAYLIB_ROOT "${CMAKE_SOURCE_DIR}/vendor/raylib")
set(RAYLIB_INCLUDE_DIR "${RAYLIB_ROOT}/src")
set(RAYLIB_LIB "${RAYLIB_ROOT}/src/raylib/libraylib.a")

# Check if library exists
if(NOT EXISTS ${RAYLIB_LIB})
    message(FATAL_ERROR "raylib library not found at ${RAYLIB_LIB}")
endif()

include_directories(${RAYLIB_INCLUDE_DIR} src/cpp)

file(GLOB SOURCES "src/cpp/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib and system dependencies
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIB} m dl pthread rt GLX OpenGL)

# Copy assets to build directory for easy access
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
