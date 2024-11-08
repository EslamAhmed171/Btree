cmake_minimum_required(VERSION 3.26)
project(BTree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BTree
        main.cpp
        Btree.cpp
)
