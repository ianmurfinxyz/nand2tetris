cmake_minimum_required(VERSION 3.20)
project(nand2tetris)
set(CMAKE_CXX_STANDARD 20)

################################################################################
# nand2tetris
################################################################################

include_directories(${PROJECT_SOURCE_DIR}/assembler/src)
include_directories(${PROJECT_SOURCE_DIR}/vm_translator/src)
include_directories(${PROJECT_SOURCE_DIR}/compiler/src)
include_directories(${PROJECT_SOURCE_DIR}/emulator/src)
add_executable(assembler
        assembler/src/main.cc
        assembler/src/cli.cc
        assembler/src/error.cc
        assembler/src/log.cc
        assembler/src/util.cc
        assembler/src/coder.cc
        assembler/src/parser.cc
        assembler/src/writer.cc)
add_executable(vm_translator vm_translator/src/main.cc)
add_executable(compiler compiler/src/main.cc)
add_executable(emulator emulator/src/main.cc)

################################################################################
# unit tests
################################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/gtest/lib)
include_directories(${PROJECT_SOURCE_DIR}/gtest/lib/googletest/include)
include_directories(${PROJECT_SOURCE_DIR}/gtest/lib/googlemock/include)
add_executable(test_assembler
        assembler/src/cli.cc
        assembler/src/error.cc
        assembler/src/parser.cc
        assembler/src/util.cc
        assembler/src/coder.cc
        assembler/test/cli_test.cc
        assembler/test/parser_test.cc
        assembler/test/coder_test.cc)
target_link_libraries(test_assembler gtest_main)
