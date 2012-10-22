/**************************************************************
* Jianlong Chai  chai.4@wright.edu  June 6th 2012             * 
* This function is mainly used to initialize the database     *
* table system, configure, and calibration.                   *
* table sdl initialization is in file SDLdb.c                 *
**************************************************************/
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


int dbinit(int dbindex)
{
	sqlite3 *conn;
	//char sql[80]="";
	char *sql;
	sqlite3_stmt *res;
	const char *tail;
	int i,steps = 0;
	double value= 0;
	int     error = 0;
	error = sqlite3_open(database, &conn);
	if (error) 
	{
		return  -1;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);

	if(dbindex == 0) 
	{
		error = sqlite3_exec(conn,"delete from configure where 1=1;",0,0,0);
		error = sqlite3_exec(conn,"delete from calibration where 1=1;",0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(1,'thread',0);"
		,0,0,0);

		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(2,'year',2012);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(3,'month',4);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(4,'day',2);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(5,'hour',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(6,'minute',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into system (id, system_property,value) values(7,'second',0);"
		,0,0,0);
	
		if ( (sql = malloc( 80 * sizeof(char) ) )== NULL)return -100; 
		for(i=1;i<=30;i++) 
		{
			sprintf(sql,"insert into calibration (id, flowbotics_value,length) values(NULL,%d,%2.2f);",steps,value);
			error = sqlite3_exec(conn,sql,0, 0, 0);
			steps = steps+25000;
			value = value + 1.5;
		}
		free(sql);


		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(1,'Depth',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(2,'Spacing',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(3,'Dwell',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(4,'Interval',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(5,'profiled',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(6,'calibrate',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(7,'count',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(8,'stopped',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(9,'last_position',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(10,'sleep',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(11,'offset',0);"
		,0,0,0);
	}

	if(dbindex == 1) 
	{
	
		if ( (sql = malloc( 80 * sizeof(char) ) )== NULL)return -100; 
		sprintf(sql,"insert into calibration (id, flowbotics_value,length) values(NULL,?,?);");
		sqlite3_prepare_v2(conn,sql,80,&res, &tail);
		sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
		error = sqlite3_exec(conn,"delete from calibration where 1=1;",0,0,0);
		for(i=1;i<=30;i++) 
		{
			sqlite3_bind_int(res,1,steps);
			sqlite3_bind_double(res,2,value);
			sqlite3_step(res);
			steps = steps+25000;
			value = value + 1.5;
			sqlite3_reset(res);
		}
		free(sql);
		sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
/*
	if ( (sql = malloc( 80 * sizeof(char) ) )== NULL)return -1; 
	for(i=1;i<=30;i++) {
	sprintf(sql,"insert into calibration (id, flowbotics_value,length) values(NULL,%d,%2.2f);",steps,value);
	error = sqlite3_exec(conn,sql,0, 0, 0);
	steps = steps+10000;
	value = value + 1.5;
	}
*/
	}

	if(dbindex == 2) 
	{
		error = sqlite3_exec(conn,"delete from configure where 1=1;",0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(1,'Depth',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(2,'Spacing',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(3,'Dwell',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(4,'Interval',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(5,'profiled',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(6,'calibrate',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(7,'count',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(8,'stopped',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(9,'last_position',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(10,'sleep',0);"
		,0,0,0);
		error = sqlite3_exec(conn,
		"insert into configure (id, flowbotics_property,value) values(11,'offset',0);"
		,0,0,0);
	}

	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
    sqlite3_close(conn);
	if (error) {
		return  -1;
	}
	
    return 1;
}
