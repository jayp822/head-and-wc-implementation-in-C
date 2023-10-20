FLAGS = -Wall -pedantic-errors

cl: head.c wc.c
	gcc -o head $(FLAGS) head.c
	gcc -o wc $(FLAGS) wc.c

run: cl
	./head
	
clean:
	rm -f head wc



