FLAGS = -Wall -pedantic-errors

cl: head.c wc.c
	gcc -o head $(FLAGS) head.c
	gcc -o wc $(FLAGS) wc.c

run: cl
	./$(NAME)
	
clean:
	rm -f head *.o
	rm -f wc *.o



