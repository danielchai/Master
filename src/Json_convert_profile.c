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
#include <ExternVars.h>
/*
typedef struct Test{
    int id;
	int value;
	double length;
} SLEN ;
*/
struct ARF;
const char *json_profile_option(int num,ARF *examp)
{
	/*
	int j;
	for (j=0;j<num;j++) printf("%d is %s %lf\n", examp[j].id,examp[j].property,examp[j].value);
	printf("%d\n",num);
	*/
	json_object * jobj = json_object_new_object();
	int i=0;
    for(i=0;i<num;i++)
	{
		json_object *jarray = json_object_new_array();
  		json_object *jfloat = json_object_new_double(examp[i].value);
  		json_object_array_add(jarray,jfloat);
 		json_object_object_add(jobj,examp[i].property, jarray);
	}
	return json_object_to_json_string(jobj);
}
