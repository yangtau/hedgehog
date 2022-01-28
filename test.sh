#!/usr/bin/env bash

formater=./src/formater

formater_test() {
    for f in test/*.hg; do
        local prefix=${f%.*}
        local suffix=${f#*.}
        if [ "$suffix" != "hg" ]; then
            continue
        fi
        echo "check formater: ${f}"
        $formater $f | diff - "${prefix}.fmt.hg"
        if [ $? -ne 0 ]; then
            echo "FAIL: formater ${f}"
        fi
    done
}

make UDEF='-DHG_DEBUG'
formater_test

