cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME MinesSweeper)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)

target_include_directories(${PROJECT_NAME} PRIVATE src/include)

message(STATUS "Configuring ${PROJECT_NAME} with SFML version ${SFML_VERSION}")
