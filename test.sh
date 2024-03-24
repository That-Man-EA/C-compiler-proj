#!/bin/bash

#set defaults to x86
GCC=x86_64-linux-gnu-gcc
TMP=tmp
RUN_LINE="qemu-x86_64-static ./$TMP"

assert() {
    expected="$1"
    input="$2"

    ./leocc "$input" > $TMP.s || exit
    $GCC -static -o $TMP $TMP.s 
    $RUN_LINE
    actual="$?"

    if [ "$actual" = "$expected" ]; then 
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1 
    fi
}

assert 1 1 
assert 42 42 
assert 47 '41+6'
assert 35 '5+20+10'
assert 13 '20    -    7     ' 
assert 100 '10+20+50-10-20+50'

echo OK
