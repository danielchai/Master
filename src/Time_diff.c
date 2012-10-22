/*************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012              *
* auto_run function is used to check the different between   *
* the current time and expected_profile time                 *
* profile_time_check function is used to determine when      *
* will be the next profile time and store it in database     *
*************************************************************/
#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <stdlib.h>
#include  <time.h>

extern int *check_time(int*date);
extern int expected_time_set(int *expected_date );
/***********************************************************
*    long auto_run(int index, int *date) function id       *
*    used to get the differences between two times         *
***********************************************************/
long auto_run(int index, int *date)
{
	time_t rawtime,endtime;
	struct tm *timeinfo;
	struct tm timeend;
	long d;
    //int year,month,day,hour,minute,second,result;
	rawtime = time(NULL);
	timeinfo = localtime(&rawtime);	
    timeend.tm_year = *date-1900;
	timeend.tm_mon = *(date+1)-1;
	timeend.tm_mday = *(date+2);
	timeend.tm_hour = *(date+3);
	timeend.tm_min = *(date+4);
	timeend.tm_sec = *(date+5);
	timeend.tm_isdst = -1;
	endtime = mktime(&timeend);

	if((rawtime == -1 )|| (endtime == -1))printf("error\n");
	else 
	{ 
		d = difftime(endtime, rawtime);
		if(index == 0)return d;
	}
	return 1;
}
long reading_hourly()
{
	int *date,hour_time[6];
	date = (int*)malloc(6 * sizeof(int));
	date = check_time(date);
	hour_time[3] = date [3] + 1;
	hour_time[0] = date [0];
	hour_time[1] = date [1];
	hour_time[2] = date [2];
	hour_time[4] = 0;	
	hour_time[5] = 0;
	free(date);
	expected_time_set(hour_time);
	return 1;
	
}
long profile_time_check(double pinterval)
{
	int *date,profile_date[6];
	date = (int*)malloc(6 * sizeof(int));
	date = check_time(date);
	
	if(pinterval == 0)
	{
	 exit(1);
	}
	profile_date[3] = date [3]/pinterval + 1;
	profile_date[3] = pinterval * profile_date[3];
	profile_date[0] = date [0];
	profile_date[1] = date [1];
	profile_date[2] = date [2];
	profile_date[4] = 0;	
	profile_date[5] = 0;
	free(date);
	expected_time_set(profile_date);
	return 1;
}
