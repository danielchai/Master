/********************************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                                 *
* This function is used to set the value in configure table                     *
* It is most used by set last position, set profile flag, set profile settings  *
*********************************************************************************/
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
int login(char * event);
int setconfig(int index,double updatevalue)
{
	sqlite3 *conn;
    int error = 0;
	int result;
   // char sql[80]="";
	char *sql;
	sql = (char*)malloc(80*sizeof(char));
	if(sql == NULL) return -100;
	
	sprintf(sql,"update configure set value = %lf where id= %d",updatevalue,index);
	login(sql);
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
