/**********************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                       *
* Json-convert.c, Json_convert_array.c, Json_convert_float.c          *
* Json_convert_profile.c Json_convert_sdl.c are all used to convert   * 
* different type of data into Json format.                            *
**********************************************************************/
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

const char *json_option(char * property , int value)
{
	json_object * jobj = json_object_new_object();
	json_object *jint = json_object_new_int(value);
 	json_object_object_add(jobj,property, jint);
	return json_object_to_json_string(jobj);
}
