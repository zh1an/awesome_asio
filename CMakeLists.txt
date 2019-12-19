cmake_minimum_required(VERSION 3.8)

project(awsome_asio)

include_directories(
        asio/asio/include
        sources/
)

add_executable(awsome_asio sources/main.cc sources/InvokeTimer.hpp)