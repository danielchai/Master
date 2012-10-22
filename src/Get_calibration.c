/**********************************************************************
* Jianlong Chai chai.4@wright.edu  June 6th 2012                      *
* This function is used to read the calibaration table from database. *
* The Records_Size is define in the header                            *
* The return value will be in Json type                               *
* It is used by API call ./master get                                 *
**********************************************************************/
#include <stdio.h>  
#include  <stdlib.h>
#include <assert.h>
#include <string.h> 
#include <errno.h>
#include <json.h>     
#include <DIO.h>
#include <tsctllib.h>
#include <sqlite3.h>
#include <Defs.h>
#include <ExternVars.h>

struct SLEN;
struct ARF;

SLEN * getall(int *num, SLEN *bexamp)
{
	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	const char *tail;
	//SLEN *bexamp;
   
    int i=0; 
	error = sqlite3_open(database, &conn);
    if (error) 
	{
    	return NULL;   
	}
	error = sqlite3_prepare_v2(conn,"select * from calibration order by length",1000, &res, &tail);
  
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
     /*
		int j;
		printf("sizeof *examp:%d sizeof examp:%d\n",sizeof(*bexamp),sizeof(bexamp));
		for (j=0;j<i;j++) printf("%d is %d %f\n", bexamp[j].id,bexamp[j].step,bexamp[j].length);
    */
		//sqlite3_exec(conn, "END TRANSACTION", 0,0,0);
	*num = i;
	sqlite3_finalize(res);
	sqlite3_close(conn);
	return bexamp;
}
