/**********************************************************
* Jianlong Chai chai.4@wright.edu  June 6th 2012          *
* This function will be used to read the setting from     *
* configure file so that the user can set the IP address  *
* for SDL or slave ID                                     *
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * read_configure()
{
	FILE * fp ;
	int i=0;
	char * tm;
	char rdbuf[128];
	char *ip_address;
	char *slave_id;
	ip_address = (char*)malloc(18*sizeof(char));
	slave_id = (char*)malloc(18*sizeof(char));
	fp = fopen("sdl.config", "r+" );
	if ( fp == 0 )
	{
		printf( "Could not open file\n" );
	}
	fgets(rdbuf,128,fp);
	if (strncmp(rdbuf,"SDL_IP",6)==0)
	{
		strncpy(ip_address,rdbuf+9,14);
		printf("%s\n",ip_address);
	}
	fgets(rdbuf,128,fp);
	if (strncmp(rdbuf,"TCP_SLAVE_ID",12)==0)
	{
		strncpy(slave_id,rdbuf+15,5);
		printf("%s\n",slave_id);
	}
	fclose( fp );
	return ip_address;
}
