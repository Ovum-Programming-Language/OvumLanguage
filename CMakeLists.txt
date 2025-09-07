cmake_minimum_required(VERSION 3.12)

project(
    ovum
    VERSION 0.1
    DESCRIPTION "Ovum programming language toolset"
    LANGUAGES CXX
)

include(cmake/SetCompilerOptions.cmake)
include(cmake/IncludeExternalLibraries.cmake)

add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)
