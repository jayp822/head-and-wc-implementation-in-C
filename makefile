FLAGS = -Wall -pedantic-errors

cl: head.c wc.c
	gcc -o head $(FLAGS) head.c
	gcc -o wc $(FLAGS) wc.c

clean:
	rm -f head wc



