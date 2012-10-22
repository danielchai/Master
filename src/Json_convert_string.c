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

const char *json_option_string(char * property , char *content)
{
	json_object * jobj = json_object_new_object();
	json_object *jstring = json_object_new_string(content);
 	json_object_object_add(jobj,property, jstring);
  	return json_object_to_json_string(jobj);
}
