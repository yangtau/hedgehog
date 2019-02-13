cmake_minimum_required(VERSION 3.10)
project(HG C)

set(CMAKE_C_STANDARD 11)

add_executable(HG expression.h environment.h expression.c environment.c oop.h variable.h
        function.c function.h
        interpreter.c
        statement.c
        value.c
        main.c variable.c
        lex.yy.c
        y.tab.c)
