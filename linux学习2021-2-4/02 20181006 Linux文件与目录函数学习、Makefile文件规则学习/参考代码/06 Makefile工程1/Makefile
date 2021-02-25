app:print.o main.o sum.o
	gcc main.o print.o sum.o -o app
print.o:print.c
	gcc print.c -c
main.o:main.c
	gcc main.c -c
sum.o:sum.c
	gcc sum.c -c
clean:
	rm app *.o -f
