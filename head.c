#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 1048576

int main(int argc, char *argv[])
{
    int opt, n, fd;
    char buffer[BUFFSIZE];
    // Default is -n 10
    int nflag = 1;
    int num = 10;

    // n: and c: specify that something is expected to come after -n and -c
    while ((opt = getopt(argc, argv, "n:c:")) != -1)
    {
        switch (opt)
        {
            case 'n':
                nflag = 1;
                num = atoi(optarg);
                break;
            case 'c':
                nflag = 0;
                num = atoi(optarg);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    //checks if num is valid
    if (num < 1)
    {
        errno = EINVAL;
        perror("Num is less than 1");
        return 0;
    }
    // -c num only
    if (nflag == 0 && argc == 3)
    {
        if ((n = read(STDIN_FILENO, buffer, num)) == -1) perror("read");
        if (write(STDOUT_FILENO, buffer, n) != n) perror("write");
    }
    // -n num only
    if (nflag == 1 && (argc == 3 || argc == 1))
    {
        for (int j = 0; j < num; ++j)
        {
            while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
            {
                for (int i = 0; i < n; ++i)
                {
                    if (buffer[i] == '\n')
                        write(STDOUT_FILENO, buffer, i + 1);

                    lseek(STDIN_FILENO, i - n + 1, i + 1);
                }
                break;
            }
        }
    }

    //for arguments after -c num or -n num
    for (; optind < argc; optind++)
    {
        // -c num [args] is used
        if (nflag == 0)
        {
            if (*argv[optind] != '-')
            {
                if ((fd = open(argv[optind], O_RDONLY)) == -1)perror("open");
                if ((n = read(fd, buffer, num)) == -1) perror("read");
                if (write(STDOUT_FILENO, buffer, n) == -1) perror("write");
                if (close(fd) == -1) perror("close");
            } else
            {
                if ((n = read(STDIN_FILENO, buffer, num)) == -1) perror("read");
                if (write(STDOUT_FILENO, buffer, n) != n) perror("write");
            }
        } else if (nflag == 1)
        {
            // -n num [args] used
            if (*argv[optind] != '-')
            {
                int bytesRead, linesRead = 0;
                if ((fd = open(argv[optind], O_RDONLY)) == -1)perror("open");

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
                                write(STDOUT_FILENO, buffer, i + 1); // Write the lines to stdout
                                close(fd); // Close the file after reading the desired number of lines
                                return 0;
                            }
                        }
                    }
                    write(STDOUT_FILENO, buffer, bytesRead); // Write the lines to stdout
                }
                close(fd);
            } else
            {
                // -n 10 only or -n num only
                for (int j = 0; j < num; ++j)
                {
                    while ((n = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0)
                    {
                        for (int i = 0; i < n; ++i)
                        {
                            if (buffer[i] == '\n')
                                write(STDOUT_FILENO, buffer, i + 1);

                            lseek(STDIN_FILENO, i - n + 1, i + 1);
                        }
                        break;
                    }
                }
            }
        }
        if (*argv[optind] == '-')
        {
            // only - is used
            while ((n = read(STDIN_FILENO, buffer, num)) == -1) perror("read");
            {
                if (write(STDOUT_FILENO, buffer, n) != n) perror("write");
                if (n == -1) perror("read");
            }
        }
    }
}

