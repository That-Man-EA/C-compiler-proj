CC=clang++
CFLAGS=-std=c++11 -g -fno-common

leocc: main.cpp
	$(CC) -c leocc.hpp tokenizer.cpp parser.cpp codegen.cpp main.cpp $(CFLAGS)
	$(CC) *.o -o leocc

test: leocc
	./test.sh 

clean:
	rm -f leocc *.o *~ tmp* *.s

.PHONY: test clean 
