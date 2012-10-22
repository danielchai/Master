/**********************************************************
* Jianlong Chai chai.4@wright.edu  June 6th 2012          *
* This function is used to enable rs485                   *
***********************************************************/
#include <stdio.h>     
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <types.h>     
#include <fcntl.h>  
#include <stat.h>    
#include <termios.h>    
#include <errno.h>      
#include <getopt.h>
#include "tsctllib.h"
#include <sys/mman.h>
#include <assert.h>
#include <strings.h>
#include <ExternVars.h>
#include "Defs.h"

extern int login(char * event);

void enable(int command)
{
	Bus *syscon;
	syscon = BusInit1(0); // SYSCON Bus
	syscon->API->Lock(syscon,WaitLock,0);
	if(command == 1) 
	{
		syscon->API->BitSet16(syscon,0xA,10);
	}
	else
	{
		syscon->API->BitClear16(syscon,0xA,10);
	}
	syscon->API->Lock(syscon,DoRelease,0);
}

