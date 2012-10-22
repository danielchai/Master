/***************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                *
* This function is used when API call ./master set_time        *
* is running.                                                  *
* In this function, it will check if the entered data or time  *
* is valid or not. If it is valid, then change clock time and  * 
* restart cron. If it is invalid, then quit                    *
****************************************************************/
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
#include <getopt.h>
#include "tsctllib.h"
#include "sqlite3.h"
#include <time.h>
#include <signal.h>
#include <errno.h>

//Added 2-10-2012 2:00pm
int set_time (int date[])
{   int year,month,day,hour,minute,second;
    year= date[0];
    month = date[1];
    day = date[2]; 
    hour = date[3];
    minute = date[4];
    second = date[5];

    /*
      this part is used to check the input data and see if it is a valid date
    */
    if(((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))&&(day > 31))return 0;
	if(((month == 4) || (month == 6) || (month == 9) || (month == 11))&&(day > 30))return 0;
    if((year%400 == 0)||((year%100 != 0)&&(year%4 == 0)))
     {if((month == 2)&&(day > 29)) return 0;}
	else if((month == 2)&&(day > 28))return 0;
	if((hour > 23) || (minute > 60) || (second > 60)) return 0;
    

	time_t rawtime,endtime;
	struct tm *timeinfo;
	struct timeval set;
    int rc;
	long d;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
  
    timeinfo->tm_year = date[0] - 1900;
    timeinfo->tm_mon = date[1] - 1;
    timeinfo->tm_mday = date[2];
    timeinfo->tm_hour = date[3];
    timeinfo->tm_min = date[4];
    timeinfo->tm_sec = date[5];

    endtime = mktime(timeinfo);
	if((endtime == -1))return 0;
	else d = difftime(endtime, rawtime);

    set.tv_sec=rawtime+d;
    set.tv_usec=0;

    rc=settimeofday(&set, NULL);
	system("/etc/init.d/cron restart");	
    if(rc==0) 
	{
        return 1;
    }
    else 
	{
        return 0;
    }
}
