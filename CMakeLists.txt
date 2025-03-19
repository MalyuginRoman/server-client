cmake_minimum_required(VERSION 3.0)
project(test)

option(test "test test1 test2 function" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0")

add_executable(server_a_test server_autorization_test.cpp)
target_include_directories(server_a_test PRIVATE include)
add_executable(client1_test client_1_test.cpp)
target_include_directories(client1_test PRIVATE include)
target_compile_definitions(
  server_a_test PRIVATE
  $<$<BOOL:${test}>:test1>
  client1_test PRIVATE
  $<$<BOOL:${test}>:test2>
)

enable_testing()
add_test(NAME server_a_test COMMAND $<TARGET_FILE:server_a_test>)
add_test(NAME client1_test COMMAND $<TARGET_FILE:client1_test>)
