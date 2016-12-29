CC = g++ -std=c++11 -Wall -Wextra -I ../../src/

all: multiarray

multiarray: multiarray.out
	./multiarray.out && echo "Test Successful." || echo "Test Failed."

multiarray.out: test_multiarray.cpp ../../src/multiarray.h
	$(CC) test_multiarray.cpp -o multiarray.out

clean:
	rm multiarray.out
