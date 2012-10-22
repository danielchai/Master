#include <stdlib.h>
#include <stdio.h>  
#include <json.h>
#include <assert.h>
#include <string.h>      
#include <DIO.h>
#include <tsctllib.h>
#include <sqlite3.h>
#include <errno.h>
#include <Defs.h>
#include <ExternVars.h>

struct System;

System * getsystem(int *num)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail;
	System  *system;
	
    int i=0; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from system order by id",1000, &res, &tail);
  
	if (error != SQLITE_OK) {
		return NULL;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
    if (  ( system = (struct System *) malloc( Records_Size * sizeof(System) ) ) == NULL)return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
     {	
        system[i].id=sqlite3_column_int(res, 0);
		system[i].property=(char *) malloc ( (strlen(sqlite3_column_text(res, 1))+1) * sizeof(char ) );
        strcpy(system[i].property,sqlite3_column_text(res, 1));
		system[i].value = sqlite3_column_int(res, 2);
		//printf("%d\n",sqlite3_column_int(res, 2));
        i++;
      }   
     		
	  /* int j;
	   printf("sizeof *examp:%d sizeof examp:%d\n",sizeof(*system),sizeof(system));
	   for (j=0;j<i;j++) printf("%d is %s %d\n", system[j].id,system[j].property,system[j].value);
	*/
		sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
      	*num = i;
		sqlite3_finalize(res);
		sqlite3_close(conn);
        return system;
}
