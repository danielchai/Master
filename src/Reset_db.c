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

int reset(int index)
{
	sqlite3 *conn;
	int     error = 0;

	error = sqlite3_open(database, &conn);
	if (error) {
		return  0;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	if(index == 0) 
	{
		error = sqlite3_exec(conn,"Drop table 'calibration'",0,0,0);
    	error = sqlite3_exec(conn,
		"create table calibration (id  integer primary key,flowbotics_value long,length double)"
		,0,0,0);

		error = sqlite3_exec(conn,"Drop table 'configure'",0,0,0);
    	error = sqlite3_exec(conn,
		"create table configure (id  integer primary key,flowbotics_property varchar(20),value double)"
		,0,0,0);
	
		error = sqlite3_exec(conn,"Drop table 'system'",0,0,0);
    	error = sqlite3_exec(conn,
		"create table system (id  integer primary key,system_property varchar(20),value integer)"
		,0,0,0);
		
		error = sqlite3_exec(conn,"Drop table 'sdl'",0,0,0);
    	error = sqlite3_exec(conn,
		"create table sdl (id  integer primary key,flowbotics_property varchar(20),value double)"
		,0,0,0);
	
	}
	if(index == 1) 
	{
		error = sqlite3_exec(conn,"Drop table 'calibration'",0,0,0);
	    error = sqlite3_exec(conn,
		"create table calibration (id  integer primary key,flowbotics_value long,length double)"
		,0,0,0);
	}
	if(index == 2) 
	{
		error = sqlite3_exec(conn,"Drop table 'configure'",0,0,0);
	    error = sqlite3_exec(conn,
		"create table configure (id  integer primary key,flowbotics_property varchar(20),value double)"
		,0,0,0);
	}
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	if (error) 
	{
		printf("error\n");
		return 0;
	}
	sqlite3_close(conn);
    return  1;
}
