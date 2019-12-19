cmake_minimum_required(VERSION 3.8)

project(timer)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../source/CMakeLists.txt)

add_executable(timer main.cc ${AWESOME_ASIO_SRCS})

