VPATH=./main:./print:./sum
CC=gcc
CFLAGS=-I./main -I./sum -I./print
app:print.o main.o sum.o
	#gcc $^ -o $@
	$(CC) -fPIC -shared -o libabc.so $^
clean:
	rm app *.o -f
