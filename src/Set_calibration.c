/*******************************************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                                            *
* This function is used to insert the calibration records to the calibration table.        *
* Before inserting the record, check_value should be called to determine if the record     *
* is reasonable                                                                            *
********************************************************************************************/
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
#include <Defs.h>

extern int login(char *event);
int set(int index,int steps,double value)
{
	sqlite3 *conn;
    int error = 0;
    //char sql[80]="";
	char *sql;
    int result = 1;

	sql = (char*)malloc(80*sizeof(char));
	sprintf(sql,"insert into calibration (id, flowbotics_value,length) values(NULL,%d,%2.2f);",steps,value);
	printf("%s\n",sql);
	login(sql);
	error = sqlite3_open(database, &conn);

	// If error == 0 then database was opened successfully. If error != 0 then there was a problem,
	// so return 0
	if (error) 
	{
		free(sql);
		return -1;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	if(result)
	{
		free(sql);
		return -1;
	}
	free(sql);
	sqlite3_close(conn);
	return 1;
}

