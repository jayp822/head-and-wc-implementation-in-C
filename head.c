#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFFSIZE 1048576

void print_error_and_exit(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int opt, n, fd;
    char buffer[BUFFSIZE];
    int nflag = 1;
    int num = 10;
    bool flagUsed = false;

    // n: and c: specify that something is expected to come after -n and -c
    while ((opt = getopt(argc, argv, "n:c:")) != -1)
    {
        switch (opt)
        {
            case 'n':
                nflag = 1;
                flagUsed = true;
                num = atoi(optarg);
                break;
            case 'c':
                nflag = 0;
                flagUsed = true;
                num = atoi(optarg);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    if (!flagUsed)
    {
        // flag not used and ./head only
        if (argc == 1)
        {
            for (int j = 0; j < num; ++j)
            {
                while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        if (buffer[i] == '\n')
                            if (write(STDOUT_FILENO, buffer, i + 1) == -1) print_error_and_exit("write");

                        if (lseek(STDIN_FILENO, i - n + 1, SEEK_CUR) == -1) print_error_and_exit("lseek");
                    }
                    if (n == -1) print_error_and_exit("read");
                    break;
                }
            }
        } else
        {
            // multiple arguments but -n or -c not used
            for (; optind < argc; optind++)
            {
                printf("\n==> %s <==\n", argv[optind]);
                if (*argv[optind] == '-')
                {
                    // - is used
                    for (int j = 0; j < num; ++j)
                    {
                        while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
                        {
                            for (int i = 0; i < n; ++i)
                            {
                                if (buffer[i] == '\n')
                                    if (write(STDOUT_FILENO, buffer, i + 1) == -1) print_error_and_exit("write");

                                if (lseek(STDIN_FILENO, i - n + 1, SEEK_CUR) == -1) print_error_and_exit("lseek");
                            }
                            if (n == -1) print_error_and_exit("read");
                            break;
                        }
                    }
                } else
                {
                    // file used
                    int bytesRead, linesRead = 0;
                    if ((fd = open(argv[optind], O_RDONLY)) == -1)print_error_and_exit("open");
                    while ((bytesRead = read(fd, buffer, BUFFSIZE)) > 0)
                    {
                        for (int i = 0; i < bytesRead; i++)
                        {
                            if (buffer[i] == '\n')
                            {
                                linesRead++;
                                if (linesRead == num)
                                {
                                    buffer[i + 1] = '\0'; // Null-terminate the string at the end of the last line
                                    if ((write(STDOUT_FILENO, buffer, i + 1)) == -1)
                                        print_error_and_exit("write"); // Write the lines to stdout
                                    if (close(fd) == -1)
                                        print_error_and_exit(
                                                "close"); //Close the file after reading the desired number of lines
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    if (flagUsed)
    {
        //-c
        if (nflag == 0)
        {
            if (isdigit(*argv[2]) && argc == 3)
            {
                //-c num
                if (read(STDIN_FILENO, buffer, num) == -1) print_error_and_exit("read");
                if (write(STDOUT_FILENO, buffer, num) != num) print_error_and_exit("write");
            } else
            {
                //-c num then multiple arguments
                for (; optind < argc; optind++)
                {
                    printf("\n==> %s <==\n", argv[optind]);

                    if (*argv[optind] == '-')
                    {
                        if (read(STDIN_FILENO, buffer, num) == -1) print_error_and_exit("read");
                        if (write(STDOUT_FILENO, buffer, num) == -1) print_error_and_exit("write");
                    } else
                    {
                        if ((fd = open(argv[optind], O_RDONLY)) == -1)print_error_and_exit("open");
                        if ((n = read(fd, buffer, num)) == -1) print_error_and_exit("read");
                        if (write(STDOUT_FILENO, buffer, n) == -1) print_error_and_exit("write");
                        if (close(fd) == -1) print_error_and_exit("close");
                    }
                }
            }
        } else
        {
            if (isdigit(*argv[2]) && argc == 3)
            {
                //-n num
                for (int j = 0; j < num; ++j)
                {
                    while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
                    {
                        for (int i = 0; i < n; ++i)
                        {
                            if (buffer[i] == '\n')
                                if (write(STDOUT_FILENO, buffer, i + 1) == -1) print_error_and_exit("write");

                            if (lseek(STDIN_FILENO, i - n + 1, SEEK_CUR) == -1) print_error_and_exit("lseek");
                        }
                        if (n == -1) print_error_and_exit("read");
                        break;
                    }
                }
            } else
            {
                for (; optind < argc; optind++)
                {
                    printf("\n==> %s <==\n", argv[optind]);

                    //-n num then multiple arguments
                    if (*argv[optind] != '-')
                    {
                        int bytesRead, linesRead = 0;
                        if ((fd = open(argv[optind], O_RDONLY)) == -1)print_error_and_exit("open");
                        while ((bytesRead = read(fd, buffer, BUFFSIZE)) > 0)
                        {
                            for (int i = 0; i < bytesRead; i++)
                            {
                                if (buffer[i] == '\n')
                                {
                                    linesRead++;
                                    if (linesRead == num)
                                    {
                                        buffer[i + 1] = '\0'; // Null-terminate the string at the end of the last line
                                        if ((write(STDOUT_FILENO, buffer, i + 1)) == -1)
                                            print_error_and_exit("write"); // Write the lines to stdout
                                        if ((close(fd)) == -1)
                                            print_error_and_exit(
                                                    "close"); // Close the file after reading the desired number of lines
                                    }
                                }
                            }
                            break;
                        }
                    } else
                    {
                        for (int j = 0; j < num; ++j)
                        {
                            while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
                            {
                                for (int i = 0; i < n; ++i)
                                {
                                    if (buffer[i] == '\n')
                                        if (write(STDOUT_FILENO, buffer, i + 1) == -1) print_error_and_exit("write");

                                    if (lseek(STDIN_FILENO, i - n + 1, SEEK_CUR) == -1) print_error_and_exit("lseek");
                                }
                                if (n == -1) print_error_and_exit("read");
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}



