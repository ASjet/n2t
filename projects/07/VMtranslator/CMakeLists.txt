cmake_minimum_required(VERSION 3.5)

project(VMtranslator)

add_executable(
    ${PROJECT_NAME}
    cpp/main.cpp
    cpp/VMparser.cpp
    cpp/CodeWriter.cpp
    cpp/utility.cpp
)

target_include_directories(
    VMtranslator
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
