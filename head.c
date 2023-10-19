#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 1048576

int main(int argc, char* argv[])
{
	int opt;
	int nflag = 1;
	int num = 10;

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
	
	if(nflag) printf("-n %d option used\n", num);
	else	  printf("-c %d option used\n", num);

	for(; optind < argc; optind++)
	{
		printf("extra arguments: %s\n", argv[optind]);
	}
	return 0;

}
