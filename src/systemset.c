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

int setsystem(int index,int updatevalue)
{
	sqlite3 *conn;
    int error = 0;
    //char sql[80]="";
	char *sql;
    int result = 1;
	
	sql = (char*)malloc(80*sizeof(char));
	if(sql == NULL)
	{
		return -100;
	}

	sprintf(sql,"update system set value = %d where id= %d",updatevalue,index);

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
  	free(sql);
	sqlite3_close(conn);
	if(result)
	{
		return -1;
	}
	return result;
}
