/**************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012               *
* This function is used for set time. When the user type in   *
* the date and time. The terminal get it in sting format.     *
* Then using this function can split the year, month,day,     *
* hour, minute,second.                                        *
**************************************************************/
#include <stdio.h>  
#include "json.h"  
#include <stdlib.h>
#include <string.h>    
#include <getopt.h>
#include "tsctllib.h"

int * parse(int *return_date,char *date,char *time)
{
 char p_date[50]="";
 char delimiters_d[] = " --";
 char *year, *month, *day;

 strcpy(p_date,date);
 month = strtok (p_date, delimiters_d);
 day = strtok (NULL, delimiters_d);
 year = strtok (NULL, delimiters_d);

 
 char p_time[50]="";
 char delimiters_t[] = " ::";
 char *hour, *minute, *second;
 strcpy(p_time,time);
 hour = strtok (p_time, delimiters_t);
 minute = strtok (NULL, delimiters_t);
 second = strtok (NULL, delimiters_t);
 //printf("%s-%s-%s ",month,day,year);
 //printf("%s:%s:%s\n",hour,minute,second);
/*
 int buf[6];
 buf[0]=atoi(year);
 buf[1]=atoi(month);
 buf[2]=atoi(day);
 buf[3]=atoi(hour);
 buf[4]=atoi(minute);
 buf[5]=atoi(second);
*/
 return_date[0]=atoi(year);
 return_date[1]=atoi(month);
 return_date[2]=atoi(day);
 return_date[3]=atoi(hour);
 return_date[4]=atoi(minute);
 return_date[5]=atoi(second);

 return return_date;

}
