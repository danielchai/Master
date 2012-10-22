/**********************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                       *
* This function is simlar with set_configure function.                *
* The major different is that set_configure can just set one argument *
* every time, but set_profile can set four profiles parameters once   *
**********************************************************************/
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

int set_profile(double depth, double spacing, double dwell, double interval)
{
	sqlite3 *conn;
    int error = 0;
    char *sql;
    int result = 1;

	
	error = sqlite3_open(database, &conn);
	// If error == 0 then database was opened successfully. If error != 0 then there was a problem,
	// so return 0
	if (error) 
	{
		return -1;
	}
	if ( (sql = malloc( 80 * sizeof(char) ) )== NULL)return -100; 
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	sprintf(sql,"update configure set value = %lf where id= 1",depth);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sprintf(sql,"update configure set value = %lf where id= 2",spacing);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sprintf(sql,"update configure set value = %lf where id= 3",dwell);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sprintf(sql,"update configure set value = %lf where id= 4",interval);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sprintf(sql,"update configure set value = 0 where id= 5");
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sprintf(sql,"update configure set value = 0 where id= 8");
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	free(sql);
	sqlite3_close(conn);

	if(result)
	{
		return -1;
	}
	return 1;
}
