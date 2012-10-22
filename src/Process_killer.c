#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPATH 127
/****************************************************
* This function is used to kill the dead processes  *
* Because the script will kill the current process, *
* We don't use this function any more               *
****************************************************/
int killprocess()
{
	char *path = (char*)malloc(MAXPATH*sizeof(char));
	path = getenv("PWD");
 	if(path!=NULL) 
	{
 		strcat(path,"/kingkong.sh");
 	}
 	else 
	{
		free(path);
  		return -1;
 	}
 	if(system(path))
	{
		return 0;
 	}
	printf("1%s\n",path);
	return 1;
}
