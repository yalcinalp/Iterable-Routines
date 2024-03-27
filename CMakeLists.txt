cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJECT)

file(GLOB ALL_SOURCES "*.c" "*.cc" "*.cpp" "*.cxx")
file(GLOB ALL_HEADERS "*.h" "*.hh" "*.hpp" "*.ipp" "*.tpp" "*.hxx" "*.ixx" "*.txx")

set(SOURCES ${ALL_SOURCES})
set(HEADERS ${ALL_HEADERS})

project(${PROJECT} LANGUAGES CXX)
add_executable(${PROJECT} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT} PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
                       $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>)
