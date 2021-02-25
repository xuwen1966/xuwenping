VPATH=./main:./print:./sum
CC=gcc
CFLAGS=-I./main -I./sum -I./print
app:print.o main.o sum.o
	gcc $^ -o $@
clean:
	rm app *.o -f
