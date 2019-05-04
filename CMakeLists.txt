cmake_minimum_required(VERSION 3.10)
project(HG C)

set(CMAKE_C_STANDARD 11)

add_executable(HG src/expression.h src/environment.h src/expression.c src/environment.c src/oop.h src/variable.h
        src/function.c src/function.h
        src/interpreter.c
        src/statement.c
        src/value.c
        src/variable.c
        src/lex.yy.c
        src/y.tab.c
        main.c
        src/interface/editor.c src/interface/editor.h)
