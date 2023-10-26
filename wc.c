#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 1048576

void print_error_and_exit(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int args= 0;
    int opt;
    int cflag = 0;
    int lflag = 0;
    int wflag = 0;
    int n;
    int fd;
    int lines = 0;
    int words = 0;
    int chars = 0;
    int tlines = 0;
    int twords = 0;
    int tchars = 0;
    char buffer[BUFFSIZE];

    while ((opt = getopt(argc, argv, "clw")) != -1)
    {
        switch (opt)
        {
            case 'c':
                cflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 'w':
                wflag = 1;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    // turns on all 3 flags by default if no flags are specified
    if (cflag + lflag + wflag == 0)
    {
        cflag = lflag = wflag = 1;
    }

    // only ./wc
    if (argc == 1)
    {
        char buffer[BUFFSIZE];
        int n;
        int characters = 0;

        while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
        {
            if (n == -1)print_error_and_exit("read");
            characters += n;
            for (int i = 0; i < n; ++i)
            {
                if (buffer[i] == ' ' && buffer[i - 1] != ' ')
                {
                    words++;
                }
                if (buffer[i] == '\n')
                {
                    lines++;
                }
            }
        }
        printf("%-5d%-5d%-5d: STANDARD INPUT", lines, words, characters);
        exit(EXIT_SUCCESS);
    }


    for (; optind < argc; optind++)
    {
        //reset each loop run
        lines = words = chars = 0;
        if (*argv[optind] == '-')
        {
            while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
            {
                for (int i = 0; i < n; ++i)
                {
                    if (buffer[i] == '\n')
                    {
                        lines++;
                    }
                    if (buffer[i] == ' ' && buffer[i - 1] != ' ')
                    {
                        words++;
                    }
                    chars = i;
                }
            }
            tlines += lines;
            twords += words;
            tchars += chars;
            if (n == -1)print_error_and_exit("read");
            if (lflag) printf("%-5d", lines);
            if (wflag) printf("%-5d", words);
            if (cflag) printf("%-5d", chars);
            printf(": STANDARD INPUT\n");
            args++;
        } else
        {
            if ((fd = open(argv[optind], O_RDONLY)) == -1) print_error_and_exit("open");
            while ((n = read(fd, buffer, BUFFSIZE)) > 0)
            {
                for (int i = 0; i < n; ++i)
                {
                    if (buffer[i] == '\n')
                    {
                        lines++;
                    }
                    if (buffer[i] == ' ' && buffer[i - 1] != ' ')
                    {
                        words++;
                    }
                    chars++;
                }
            }
            tlines += lines;
            twords += words;
            tchars += chars;

            if (n == -1)print_error_and_exit("read");
            if ((close(fd)) == -1) print_error_and_exit("close");
            if (lflag) printf("%-5d", lines);
            if (wflag) printf("%-5d", words);
            if (cflag) printf("%-5d", chars);
            printf(": %s\n", argv[optind]);
            args++;
        }
    }
    if(args > 1)
    {
        if (lflag) printf("%-5d", tlines);
        if (wflag) printf("%-5d", twords);
        if (cflag) printf("%-5d", tchars);
        printf(": total");
    }
    exit(EXIT_SUCCESS);
}
