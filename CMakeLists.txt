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
add_executable(vm_translator
        vm_translator/src/main.cc
        vm_translator/src/tokenizer.cc
        vm_translator/src/tokenizer.hh
        vm_translator/src/parser.cc
        vm_translator/src/parser.hh
        vm_translator/src/coder.cc
        vm_translator/src/coder.hh
        vm_translator/src/writer.cc
        vm_translator/src/writer.hh
        vm_translator/src/error.cc
        vm_translator/src/error.hh
        vm_translator/src/log.cc
        vm_translator/src/log.hh
        vm_translator/test/tokenizer_test.cc)
add_executable(compiler compiler/src/main.cc)
add_executable(emulator emulator/src/main.cc)

################################################################################
# unit tests
################################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/gtest/lib)
include_directories(${PROJECT_SOURCE_DIR}/gtest/lib/googletest/include)
include_directories(${PROJECT_SOURCE_DIR}/gtest/lib/googlemock/include)
add_executable(test_nand2tetris
        assembler/src/cli.cc
        assembler/src/error.cc
        assembler/src/parser.cc
        assembler/src/util.cc
        assembler/src/coder.cc
        assembler/test/cli_test.cc
        assembler/test/parser_test.cc
        assembler/test/coder_test.cc
        vm_translator/src/error.cc
        vm_translator/src/tokenizer.cc
        vm_translator/test/tokenizer_test.cc)
target_link_libraries(test_nand2tetris gtest_main)