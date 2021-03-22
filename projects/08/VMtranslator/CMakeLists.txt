cmake_minimum_required(VERSION 3.5)

project(VMtranslator)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/VMparser.cpp
    src/CodeWriter.cpp
    src/utility.cpp
)

target_include_directories(
    VMtranslator
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_compile_options(-O0 -g)