cmake_minimum_required(VERSION 3.20)
project(black_red_tree LANGUAGES CXX)

enable_testing()

add_executable(test_bs test/test_bs.cpp)

target_include_directories(test_bs PRIVATE "{CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(test_bs PRIVATE cxx_std_20)

add_test(NAME bst_test COMMAND test_bs)

add_executable(test_br test/test_br.cpp)
target_include_directories(test_br PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(test_br PRIVATE cxx_std_20)

add_test(NAME BRtree_test COMMAND test_br)

add_executable(test_thread test/test_thread.cpp)
target_include_directories(test_thread PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(test_thread PRIVATE cxx_std_20)

add_test(NAME Thread_test COMMAND test_thread)
