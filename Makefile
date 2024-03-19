CC=clang++
CFLAGS=-std=c++11 -g -fno-common

leocc: main.cpp
	$(CC) -o leocc main.cpp $(LDFLAGS)

test: leocc
	./test.sh 

clean:
	rm -f leocc *.o *~ tmp* *.s

.PHONY: test clean 
