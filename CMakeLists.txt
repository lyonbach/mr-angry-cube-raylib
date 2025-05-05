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
set(RAYGUI_INCLUDE_DIR "./vendor/raygui/src")
set(RAYINI_INCLUDE_DIR "./vendor/rini/src")
set(RAYLIB_LIB_DIR "./vendor/raylib/src")

# Includes raylib
include_directories(
    ${RAYLIB_INCLUDE_DIR}
    ${RAYGUI_INCLUDE_DIR}
    ${RAYINI_INCLUDE_DIR})

# Link raylib library
target_link_directories(${PROJECT_NAME} PUBLIC ${RAYLIB_LIB_DIR})
target_link_libraries(${PROJECT_NAME} raylib)

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-narrowing)

# Copy models, shaders, textures
set(FOLDERS "models" "shaders" "textures")
foreach(FOLDER ${FOLDERS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/${FOLDER}"
        )
    else()
        message(WARNING "Directory ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER} does not exist. Skipping copy.")
    endif()
endforeach()

# Copy configuration file to main folder
set(INI_FILE_NAME "game.ini")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/${INI_FILE_NAME}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${INI_FILE_NAME}"
)
