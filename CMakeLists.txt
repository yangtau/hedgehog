cmake_minimum_required(VERSION 3.10)
project(HG C)

set(CMAKE_C_STANDARD 11)

add_executable(HG main.c hedgehog.h environment.c expression.c interpreter.c statement.c value.c statement.c lex.yy.c y.tab.c)