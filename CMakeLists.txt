cmake_minimum_required(VERSION 3.20)
project(black_red_tree LANGUAGES CXX)

enable_testing()

add_executable(test_bs test/test_bs.cpp)

target_include_directories(test_bs PRIVATE "{CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(test_bs PRIVATE cxx_std_17)

add_test(NAME bst_test COMMAND test_bs)