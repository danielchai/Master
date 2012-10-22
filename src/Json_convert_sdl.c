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
/*
typedef struct Test{
    int id;
	int value;
	double length;
} SLEN ;
*/
struct SLEN;
struct ARF;
const char *json_sdl_option(int num,SLEN *examp)
{
	json_object * jobj = json_object_new_object();
	int i=0;
	char *tmp;
	//if (  ( m = malloc( 10*sizeof(char) ) ) == NULL)return -1; 
	for(i=0;i<num;i++)
	{
		tmp = (char*)malloc(10*sizeof(char));
		json_object *jarray = json_object_new_array();
  		json_object *jstring1 = json_object_new_int(examp[i].id);
  		json_object *jstring2 = json_object_new_int(examp[i].step);
  		json_object *jfloat = json_object_new_double(examp[i].length);
  		//printf("%d %d\n",examp[i].id,examp[i].length);
  		json_object_array_add(jarray,jstring1);
  		json_object_array_add(jarray,jstring2);
  		json_object_array_add(jarray,jfloat);
        sprintf(tmp,"Parameter%d",examp[i].id);
 		json_object_object_add(jobj,tmp,jarray);
		free(tmp);
	}
		//free(m);
	return json_object_to_json_string(jobj);
}
