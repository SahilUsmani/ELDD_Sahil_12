
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t WR_Buff[50];
int8_t RD_Buff[50];


int main()
{
	int fd;
	
	fd = open("/dev/driver_device1", O_RDWR);
	if(fd < 0)
	{
		printf("Cannot open the file\n");
		return 0;
	}

	while(1)
	{
		int cmd = 0;
		printf("Choose your option\n");
		printf("1. Write\n");
		printf("2. Read\n");
		printf("3. Exit\n");

		scanf(" %d", &cmd);
		printf("Your Option %d", cmd);

		switch(cmd)
		{
			case 1:
				printf("Enter your data\n");
				scanf(" %[^/t/n]s", WR_Buff)
				write(fd, WR_Buff, sizeof(WR_Buff));
				printf("Data written successfully\n");
				break;

			case 2:
				printf("Data Reading\n");
				read(fd, RD_Buff, 50);
				printf("Data is %s\n", RD_Buff);
				break;

			case 3:
				printf("Exiting\n");
				exit(0);
				break;

			default:
				printf("Enter Valid Option\n");
				break;
		}
	}

	close(fd);
	return 0;

}
