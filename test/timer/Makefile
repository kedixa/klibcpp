CC = g++ -std=c++11 -Wall -Wextra -I ../../src/

all: timer

timer: timer.out
	./timer.out && echo "Test Successful." || echo "Test Failed."

timer.out: test_timer.cpp ../../src/timer.h
	$(CC) test_timer.cpp -o timer.out

clean:
	rm timer.out
