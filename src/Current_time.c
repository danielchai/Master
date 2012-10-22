/************************************************************
* Jianlong Chai  chai.4@wright.edu June 6th 2012            *
* This function is similar with Check_time.c                *
* The difference is the return value type.                  *
* This function return the string type which is specified   *
* by the API call master get_time                           *
************************************************************/
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
char *current_time(char *sdate)
{
	time_t rawtime;
	char buffer[80];
	struct tm *timeinfo;
	rawtime = time ( NULL );
	timeinfo = localtime ( &rawtime );
	strftime (buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo); 
	return strcat(sdate,buffer);
}

