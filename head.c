#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 1048576

int main(int argc, char *argv[])
{
	int opt;
	int fd;
	int n;
	char buffer[BUFFSIZE];
	off_t size;
    // Default is -n 10
	int nflag = 1;
	int num = 10;

    // n: and c: specify that something is expected to come after -n and -c
	while((opt = getopt(argc, argv, "n:c:")) != -1)
	{
		switch(opt)
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
	
	if(nflag && num > 0)
	{
		for(; optind < argc; optind++)
		{
			fd = open(argv[optind], O_RDONLY);
			if(fd == -1) perror("open");
			while((n = read(fd, buffer, num)) > 0)
			{
				if(write(STDOUT_FILENO, buffer, n) != n) perror("write");
			}
			if(n == -1) perror("read");
		}
//		if(close(argv[optind]) == -1) perror("close");
		
		return 0;
	} else if(nflag == 0 && num > 0)
	{
		//-c implemented
		for(; optind < argc; optind++)
		{
			fd = open(argv[optind], O_RDONLY);
			if(fd == -1) perror("open");
			if((n = read(fd, buffer, num)) == -1 ) perror("read");;
			if(write(STDOUT_FILENO, buffer, n) != n) perror("write");;
			if(close(fd) == -1) perror("close");
		}
		return 0;
	}else
	{
		errno = 22;
		perror("Invalid command line arguments try again");
	}
}
