cmake_minimum_required(VERSION 3.22)
project(LFAF)

set(CMAKE_CXX_STANDARD 20)

add_executable(LFAF main.cpp Grammar/grammar.h Automata/automata.h DSL/lexer.h DSL/parser.h Grammar/CNF.h Grammar/classification.h Grammar/rule.h)
