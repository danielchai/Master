/******************************************************************
* Jianlong Chai  chai.4@wright.edu  June 6th 2012                 *
* This function is used to read the table configure in database.  *
* The main purpose of this function is returning proifle settings *
* It is used by API call ./master profile_get                     *
******************************************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <json.h>
#include <assert.h>
#include <string.h>      
#include <DIO.h>
#include <tsctllib.h>
#include <sqlite3.h>
#include <errno.h>
#include <Defs.h>
#include <ExternVars.h>

struct SLEN;
struct ARF;

ARF * getconfig(int *num, ARF *dexamp)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail;
	//ARF  * dexamp;
	
    int i=0; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from configure order by id",1000, &res, &tail);
  
	if (error != SQLITE_OK) 
	{
		return NULL;
	}
	sqlite3_exec(conn, "BEGIN TRANSACTION",0,0,0);
    if (  ( dexamp = malloc( Records_Size * sizeof(ARF) ) ) == NULL)return NULL; 
    while (sqlite3_step(res) == SQLITE_ROW) 
     {	
        dexamp[i].id=sqlite3_column_int(res, 0);
		dexamp[i].property=(char *) malloc ( (strlen(sqlite3_column_text(res, 1))+1) * sizeof(char ) );
        strcpy(dexamp[i].property,sqlite3_column_text(res, 1));
		dexamp[i].value = sqlite3_column_double(res, 2);
        i++;
      }   
     /*		
	   int j;
	   printf("sizeof *examp:%d sizeof examp:%d\n",sizeof(*dexamp),sizeof(dexamp));
	   for (j=0;j<i;j++) printf("%d is %s %lf\n", dexamp[j].id,dexamp[j].property,dexamp[j].value);
	*/
	sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
    *num = i;
	sqlite3_finalize(res);
	sqlite3_close(conn);
    return dexamp;
}
