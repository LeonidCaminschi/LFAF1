cmake_minimum_required(VERSION 3.22)
project(LFAF)

set(CMAKE_CXX_STANDARD 20)

add_executable(LFAF main.cpp grammar.h automata.h)
