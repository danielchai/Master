/**********************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                       *
* This file contains all the functions used to control the sdl table  *
* in database lr.sl3                                                  *
***********************************************************************/
#include <stdio.h>  
#include <json.h>
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
#include <ExternVars.h>

struct SLEN;
struct ARF;
int resetsdl()
{
	sqlite3 *conn;
	int     error = 0;

	error = sqlite3_open(database, &conn);
	if (error) 
	{
		return  0;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	error = sqlite3_exec(conn,"Drop table 'sdl'",0,0,0);
    error = sqlite3_exec(conn,
	"create table sdl (id  integer primary key,created_at integer,value double)"
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
int get_count()
{
	sqlite3 *conn;
    int error = 0;
	int result;
	char *sql;
	sql = (char*)malloc(100*sizeof(char));
	if(sql == NULL){printf("error\n"); return -100;}
	sprintf(sql,"select count(*) from sdl");
	//printf("%s\n",sql);

	error = sqlite3_open(database, &conn);

	// If error == 0 then database was opened successfully. If error != 0 then there was a problem,
	// so return 0
	if (error) 
	{
		printf("error\n");
		free(sql);
		return -1;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
	result = sqlite3_exec(conn,sql,0, 0, 0);
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
  	if (result)
	{
		printf("error2\n");
		return -1;
	} 
	free(sql);
	printf("rows is %d\n",result);
	sqlite3_close(conn);
	return 1;

}

int setsdl(int time_stamp,double updatevalue)
{
	sqlite3 *conn;
    int error = 0;
	int result;
	char *sql;
	sql = (char*)malloc(100*sizeof(char));
	if(sql == NULL){printf("error\n"); return -100;}
	sprintf(sql,"insert into sdl (id, created_at,value) values(NULL,'%d',%lf);",time_stamp,updatevalue);
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
/*ARF * getsdl(int *num, ARF *dexamp)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail;
    int i=0; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from sdl order by id",1000, &res, &tail);
  
	if (error != SQLITE_OK) 
	{
		return NULL;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
    if ( ( dexamp = malloc( Records_Size * sizeof(ARF) ) ) == NULL)return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
	{	
		dexamp[i].id=sqlite3_column_int(res, 0);
		dexamp[i].property=(char *) malloc ( (strlen(sqlite3_column_text(res, 1))+1) * sizeof(char ) );
		strcpy(dexamp[i].property,sqlite3_column_text(res, 1));
		dexamp[i].value = sqlite3_column_double(res, 2);
		i++;
	}   

	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
    *num = i;
	sqlite3_finalize(res);
	sqlite3_close(conn);
    return dexamp;
}*/
SLEN * getsdl(int *num, SLEN *bexamp)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail;
    int i=0; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from sdl order by id",1000, &res, &tail);
  
	if (error != SQLITE_OK) {
		return NULL;
	}
	//sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
    if (  ( bexamp =  (struct Test *) malloc( Records_Size * sizeof(SLEN) ) ) == NULL)return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
    {	
        bexamp[i].id=sqlite3_column_int(res, 0);
        bexamp[i].step=sqlite3_column_int64(res, 1);
		bexamp[i].length = sqlite3_column_double(res, 2);
        i++;
    }     
	/*	int j;
		printf("sizeof *examp:%d sizeof examp:%d\n",sizeof(*bexamp),sizeof(bexamp));
		for (j=0;j<i;j++) printf("%d is %d %f\n", bexamp[j].id,bexamp[j].step,bexamp[j].length);
		//sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	*/
	*num = i;
	sqlite3_finalize(res);
	sqlite3_close(conn);
	return bexamp;
}
SLEN * getsdl_condition(int *num, SLEN *bexamp,int created_time)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	char *sql;
	int error = 0;
	const char *tail;
    int i=0; 
	sql = (char*)malloc(100*sizeof(char));
	if(sql == NULL){printf("error\n"); return -100;}
	sprintf(sql,"select * from sdl where created_at >= %d",created_time);
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,sql,1000, &res, &tail);
  
	if (error != SQLITE_OK) {
		return NULL;
	}
	//sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
    if (  ( bexamp =  (struct Test *) malloc( Records_Size * sizeof(SLEN) ) ) == NULL)return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
    {	
        bexamp[i].id=sqlite3_column_int(res, 0);
        bexamp[i].step=sqlite3_column_int64(res, 1);
		bexamp[i].length = sqlite3_column_double(res, 2);
        i++;
    }     
	/*	int j;
		printf("sizeof *examp:%d sizeof examp:%d\n",sizeof(*bexamp),sizeof(bexamp));
		for (j=0;j<i;j++) printf("%d is %d %f\n", bexamp[j].id,bexamp[j].step,bexamp[j].length);
		//sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	*/
	*num = i;
	sqlite3_finalize(res);
	sqlite3_close(conn);
	return bexamp;
}
int get_last_sdl_record()
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	char *sql;
	int error = 0;
	int created_time;
	const char *tail;
    int i=0; 
	sql = (char*)malloc(100*sizeof(char));
	if(sql == NULL){printf("error\n"); return -100;}
	
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from sdl order by id desc limit 1;",1000, &res, &tail);
  
	if (error != SQLITE_OK) {
		return NULL;
	}
    while (sqlite3_step(res) == SQLITE_ROW) 
    {	
       created_time =sqlite3_column_int64(res, 1);
    }     
	//printf("%d\n",created_time);
	sqlite3_finalize(res);
	sqlite3_close(conn);
	return created_time;
}
