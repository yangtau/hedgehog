cmake_minimum_required(VERSION 3.10)
project(HG C)

set(CMAKE_C_STANDARD 11)

add_executable(HG main.c create.c evaluate.c interpreter.c value.c lex.yy.c y.tab.c)