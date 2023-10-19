FLAGS = -Wall -pedantic-errors

cl: head.c wc.c
	gcc -o head $(FLAGS) head.c
	gcc -o wc $(FLAGS) wc.c

cl: head.o wc.o
	gcc -o head head.o
	gcc -o wc wc.o

head.o: head.c
	gcc -c $(FLAGS) head.c

wc.o: wc.c
	gcc -c $(FLAGS) wc.c


run: cl
	./head
	./wc
	
clean:
	rm -f head *.o
	rm -f wc *.o



