cmake_minimum_required(VERSION 3.21)

# Config
set(PROJECT_NAME "boolkaloader")
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

project(${PROJECT_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

message(STATUS "Building ${PROJECT_NAME} v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} for ${CMAKE_SYSTEM_NAME}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE PROJECT_SRC "src/*.cpp")

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE "ANDROID")
    target_link_libraries(${PROJECT_NAME} "log")

    install(TARGETS ${PROJECT_NAME} LIBRARY)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_C_FLAGS "-m32")
    set(CMAKE_CXX_FLAGS "-m32")
    set(CMAKE_MODULE_LINKER_FLAGS "-m32")
    set(CMAKE_SHARED_LINKER_FLAGS "-m32")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/src/version.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
        @ONLY
    )

    add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC} "${CMAKE_CURRENT_BINARY_DIR}/version.rc")

    target_compile_definitions(${PROJECT_NAME} PRIVATE "WIN32" "_WINDOWS")
endif()