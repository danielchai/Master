#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int winch_sleep(long second)
{
    char *sleep_time;
    FILE * sleep_file;
	
    //sleep_file = fopen("/home/sampler/sleep.txt","w");
	sleep_file = fopen("/root/sleep.flag","w");
	sleep_time = (char*)malloc(10*sizeof(char));
	if(sleep_file == 0)
	{
		printf("Error in opening the file\n");
		return -1;
	}
	sprintf(sleep_time,"%d",second);
	fprintf(sleep_file,sleep_time);
    fclose(sleep_file);
    
    return 1;
}

