cmake_minimum_required(VERSION 3.8)

project(block_tcp_client)

add_executable(${PROJECT_NAME} main.cc)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../source/CMakeLists.txt)
