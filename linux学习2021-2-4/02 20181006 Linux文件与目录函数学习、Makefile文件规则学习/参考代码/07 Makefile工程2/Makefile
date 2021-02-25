app:print.o main.o sum.o
	gcc $^ -o $@
%.o:%.c
	gcc $< -c
clean:
	rm app *.o -f
