NAME = cat
FLAGS = -Wall -pedantic-errors

cl: $(NAME).c
	gcc -o $(NAME) $(FLAGS) $(NAME).c

run: cl
	./$(NAME) - file1.txt file1.txt file1.txt file2.txt
clean:
	rm -f $(NAME)



