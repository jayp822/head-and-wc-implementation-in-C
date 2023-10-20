FLAGS = -Wall -pedantic-errors

cl: head wc

head: head.c
	gcc -o head $(FLAGS) head.c

wc: wc.c
	gcc -o wc $(FLAGS) wc.c

run: cl
	./head - file1.txt file2.txt
	
clean:
	rm -f head *.o
	rm -f wc *.o



