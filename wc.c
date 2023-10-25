#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

/* Adapted from https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
 * Can be used as starter code for the wc utility program
 */
int main(int argc, char *argv[])
{
    int opt;
    int cflag = 0;
    int lflag = 0;
    int wflag = 0;

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
    } // if

    int file = 0;
    int totalBytes = 0;
    int totalWords = 0;
    int totalLines = 0;
    for (int i = optind; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");
        int lineCount = 0;
        int wordCount = 0;
        int successfulRead;
        int bytes = 0;
        char buffer[4096];
        if (fp != NULL)
        { // checking if the file existed or not.
            // the loop keep repeating until it reaches the end of the file
            file++;
            while (!feof(fp))
            {
                //this line is for read data from the file to the buffer array and return number of
                // bytes successfully read.
                successfulRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
                //loop based on the amount of characters read which is 1 byte for 1 char.
                for (int i = 0; i < successfulRead; i++)
                {
                    if (lflag)
                    {
                        //check and count for lines.
                        if (buffer[i] == '\n')
                        {
                            lineCount++;
                        }
                    }
                    if (wflag)
                    {
                        if (isspace(buffer[i]) && !isspace(buffer[i + 1]))
                        {
                            wordCount++;
                        }
                    }
                    if (cflag)
                    {
                        bytes = successfulRead;
                    }
                }
            }
            totalBytes += bytes;
            totalLines += lineCount;
            totalWords += wordCount;
            if (lflag)
            {
                printf("%3d", lineCount);
            } // if
            if (wflag)
            {
                printf("%4d", wordCount);
            } // if
            if (cflag)
            {
                printf("%4d", bytes);
            }
            printf(" %s", argv[i]);
            printf("\n");
        }
    }
    if (file > 1)
    {
        printf("%3d%4d%4d", totalLines, totalWords, totalBytes);
        printf(" total\n");
    }

    // optind is for the extra arguments
    // which are not parsed
    for (; optind < argc; optind++)
    {
        printf("extra arguments: %s\n", argv[optind]);
    }

    return 0;
}
