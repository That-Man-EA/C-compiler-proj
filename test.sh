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

assert 0 0 
assert 42 42 
assert 25 '5+20'
assert 15 '5+20-10'
assert 25 '5 + 20'
assert 25 '5+ 20'
assert 25 '5 +20'
assert 10 '5 * 2'
assert 11 '1 + 5 * 2'
assert 0 '5 / 10'
assert 2 '10 / 5'
assert 8 '1 + 10 / 5 * 3 + 1'
assert 2 '1 + 10 / 5 * 3 + 1 - 2 * 3'
assert 2 '1 + 10 / 5 * 3 * 1 + 1 - 2 * 3'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 10 '+10'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'

assert 1 '0 < 1'
assert 1 '0 <= 1'
assert 1 '1 > 0'
assert 1 '1 >= 0'
assert 1 '0 == 0'
assert 1 '0 != 1'

assert 0 '0==1'
assert 1 '42==42'
assert 1 '0!=1'
assert 0 '42!=42'

assert 1 '0<1'
assert 0 '1<1'
assert 0 '2<1'
assert 1 '0<=1'
assert 1 '1<=1'
assert 0 '2<=1'

assert 1 '1>0'
assert 0 '1>1'
assert 0 '1>2'
assert 1 '1>=0'
assert 1 '1>=1'
assert 0 '1>=2'

echo OK
