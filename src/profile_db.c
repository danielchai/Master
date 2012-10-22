/****************************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                             *
* These three functions are used to deal with the profile expected_time     *
* Reset the database expected_time to zero                                  *
* Initialize the expected_time to 1/1/2012 00:00:00                         *
* Set the expected_time to the time                                         *
****************************************************************************/
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
#include <DIO.h>
#include <Time.h>
#include <tsctllib.h>
#include <Bus.h>
#include <TWI.h>
#include <sqlite3.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <Defs.h>

int expected_time_reset()
{
	sqlite3 *conn;
	int     error = 0;

	error = sqlite3_open(database, &conn);
	if (error) 
	{
		printf("error\n");
		return  0;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	error = sqlite3_exec(conn,"Drop table 'expected_time'",0,0,0);
    error = sqlite3_exec(conn,
	"create table expected_time(id  integer primary key,Year integer,Month integer,Day integer, Hour integer,Minute integer,Second integer)"
	,0,0,0);
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	if (error) 
	{
		printf("error\n");
		return 0;
	}
	sqlite3_close(conn);
    return  1;
}

int expected_time_init()
{
	sqlite3 *conn;
	int     error = 0;
	error = sqlite3_open(database, &conn);
	if (error) 
	{
		return  -1;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	error = sqlite3_exec(conn,"delete from expected_time where 1=1;",0,0,0);
	error = sqlite3_exec(conn,
	"insert into expected_time (id,Year,Month,Day, Hour,Minute,Second) values(1,2012,1,1,0,0,0);"
	,0,0,0);
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
    sqlite3_close(conn);
	if (error) 
	{
		return  -1;
	}	
    return 1;
}
int expected_time_set(int *expected_date )
{
	sqlite3 *conn;
    int error = 0;
	int result;
	char *sql;
	sql = (char*)malloc(160*sizeof(char));
	if(sql == NULL) return -100;
	sprintf(sql,"update expected_time set Year = %d,Month = %d,Day = %d,Hour = %d, Minute = %d, Second = %d where id= 1",
			expected_date[0],expected_date[1],expected_date[2],expected_date[3],expected_date[4],expected_date[5]);
	//printf("%s\n",sql);

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
  	if (result)
	{
		return -1;
	} 
	free(sql);
	sqlite3_close(conn);
	return 1;
}
int * expected_time_get(int *expected_date)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from expected_time order by id",1000, &res, &tail);
	if (error != SQLITE_OK) 
	{
		return NULL;
	}
   // if ( ( expected_date =  (int *) malloc( 20 * sizeof(int) ) == NULL))return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
    {
		expected_date[0] = sqlite3_column_int(res, 1);
        expected_date[1] = sqlite3_column_int(res, 2);
		expected_date[2] = sqlite3_column_int(res, 3);
	    expected_date[3] = sqlite3_column_int(res, 4);
        expected_date[4] = sqlite3_column_int(res, 5);
		expected_date[5] = sqlite3_column_int(res, 6);
		
    }    
   /* printf("Year = %d,Month = %d,Day = %d,Hour = %d, Minute = %d, Second = %d where id= 1",expected_date[0],expected_date[1],expected_date[2],expected_date[3],expected_date[4],expected_date[5]);*/
	sqlite3_finalize(res);
	sqlite3_close(conn);
	return expected_date;
}
