#!/usr/bin/env bash

command=./src/formater

test() {
    $command -e "fn(){}()"
    $command -e "x[1],y.x.x=arr[y.x],x.y[9].x"
    $command -e "fn(){ return }()"
    $command -e "if a {}"
    $command -e "if a { if b {} } else {}"
    $command -e "fn fac(){}; fac()"
    $command -e "G={}; a = {}.a; a = {}[9]"
    $command -e "G={1, 2, 3, 4}; G={key:value, hello:world}"
}

test_error() {
    $command -e "if () {} else {}"
    $command -e "fn ()()"
    $command -e "G={1, key:value}"
}

make UDEF='-DHG_DEBUG' > /dev/null 2>&1 
test
test_error

make > /dev/null 2>&1 
test
test_error

