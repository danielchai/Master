/******************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012       *
* These fucntions are all related to the logfile.     * 
* They are used to record the action of command and   *
* write these action in log.txt coresponding with     *
*  timestamp                                          *
*******************************************************/
#include <stdio.h>  
#include <assert.h>
#include <sys/mman.h>   
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    
#include <sys/types.h> 
#include <sys/stat.h>   
#include <fcntl.h>     
#include <termios.h>    
#include <errno.h>      
#include <getopt.h>
#include "DIO.h"
#include "Time.h"
#include "tsctllib.h"
#include "Bus.h"
#include "TWI.h"
#include "sqlite3.h"
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <Defs.h>

char * check_timer()
{
	time_t rawtime;
	struct tm * timeinfo;
	int year, month ,day,hour,minute,second;
	char * weekday[] = { "Sunday", "Monday",
                       "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday"};
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	year = timeinfo->tm_year+1900;
	month = timeinfo->tm_mon +1;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	minute = timeinfo->tm_min;
	second = timeinfo->tm_sec;
    char buf[80]="";
	sprintf (buf,"--%d/%d/%d %s %d:%d:%d", year,month,day,weekday[timeinfo->tm_wday],hour,minute,second);
	return buf;
}

int login(char * event)
{
FILE * fp ;
char * tm;
//char buf[80] = "";
char rdbuf[18];
char wbuf[90]="";
char time[40]="";
char constbuf[18] = "Winch Version 1.0";
//tm = buf;

fp = fopen(loginfile, "a+" );

if ( fp == 0 )
{
	printf( "Could not open file\n" );
}


strcpy(wbuf,event);
tm = check_timer();
strcpy(time,tm);
strcat(wbuf,time);


fgets(rdbuf,18,fp);
if (strcmp(rdbuf,constbuf)==0)
{
	fprintf(fp,wbuf);
	fprintf(fp,"\n");
}
else
{
	fprintf(fp,"Winch Version 1.0\n");
	fprintf(fp,wbuf);
	fprintf(fp,"\n");
}

fclose( fp );
return 0;
}
