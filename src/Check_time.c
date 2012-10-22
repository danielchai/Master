/**************************************************
*  Jianlong Chai (chai.4@wright.edu) June 6th 2012*
* This function is used to check the current time *
* It is used in profile to sychronize the time    *
***************************************************/
#include <stdio.h>  
#include "json.h"
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
int *check_time(int*date)
{
	time_t rawtime;
	struct tm * timeinfo;
	int year, month ,day,hour,minute,second;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	year = timeinfo->tm_year+1900;
	month = timeinfo->tm_mon +1;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	minute = timeinfo->tm_min;
	second = timeinfo->tm_sec;
	date[0] = year;
	date[1] = month;
	date[2] = day;
	date[3] = hour;
	date[4] = minute;
	date[5] = second;
	//printf ("Today is  %d/%d/%d  %s %d:%d:%d\n", year,month,day,weekday[timeinfo->tm_wday],hour,minute,second);
	//printf ("Today is  %d/%d/%d  %s %d:%d:%d\n", date[0],date[1],date[2],weekday[timeinfo->tm_wday],date[3],date[4],date[5]);
	return date;
}

