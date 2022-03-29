cmake_minimum_required(VERSION 3.20)
project(OpenGL)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

find_package(OpenGL REQUIRED)

if(WIN32)
    find_library(
            ASSIMP_LIBRARY
            NAMES
            freetype
            PATHS
            external/freetype/bin)
    set(FREETYPE_INCLUDE_DIRS external/freetype/include)
    find_library(
            FREETYPE_LIBRARY
            NAMES
            assimp
            PATHS
            external/assimp/bin)
    set(ASSIMP_INCLUDE_DIRS external/assimp/include)
else()
    find_package(Freetype REQUIRED)
    find_package(assimp REQUIRED)
endif()

file(GLOB_RECURSE SRC_SOURCES src/*.cpp)
file(GLOB_RECURSE SRC_HEADERS src/*.hpp)
set(HEADER_FILES pch.hpp)

add_executable(${PROJECT_NAME} ${SRC_SOURCES} ${SRC_HEADERS} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
        ${ASSIMP_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        # static link
        glfw
        glm
        glad
        stb
        entt
        magic_enum

        # dynamic link
        ${ASSIMP_LIBRARY}
        ${FREETYPE_LIBRARY}
        OpenGL::GL
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC ${HEADER_FILES})