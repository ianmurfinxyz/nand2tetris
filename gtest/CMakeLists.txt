project(gtests)

add_subdirectory(lib)
include_directories(lib/googletest/include lib/googletest)

add_executable(gtests_run ../assembler/test/test.cc)
target_link_libraries(gtests_run gtest gtest_main)
