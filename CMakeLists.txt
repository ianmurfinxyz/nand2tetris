cmake_minimum_required(VERSION 3.20)
project(nand2tetris)
set(CMAKE_CXX_STANDARD 20)
include_directories(${PROJECT_SOURCE_DIR}/assembler/src)
include_directories(${PROJECT_SOURCE_DIR}/virtual_machine/src)
include_directories(${PROJECT_SOURCE_DIR}/compiler/src)
include_directories(${PROJECT_SOURCE_DIR}/emulator/src)
add_executable(assembler
        assembler/src/main.cc
        assembler/src/cli.cc
        assembler/src/error.cc
        assembler/src/log.cc
        assembler/src/tokenizer.cc
        )
add_executable(virtual_machine virtual_machine/src/main.cc)
add_executable(compiler compiler/src/main.cc)
add_executable(emulator emulator/src/main.cc)
