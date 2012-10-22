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

extern int login(char *event);
int enable_wakeup(int command)
{
	Bus *syscon;
	syscon = BusInit1(0); // SYSCON Bus
	syscon->API->Lock(syscon,WaitLock,0);
	if(command == 1) 
	{
		syscon->API->BitSet16(syscon,0xA,8);
		syscon->API->BitSet16(syscon,0xA,0);
		syscon->API->BitSet16(syscon,0x2,14);
	}
	else
	{
		
		syscon->API->BitClear16(syscon,0xA,8);
		syscon->API->BitClear16(syscon,0xA,0);	
		syscon->API->BitClear16(syscon,0x2,14);
	}
	int i = syscon->API->BitGet16(syscon,0xA,8);
	int j = syscon->API->BitGet16(syscon,0xA,0);
	printf("The bit 8 is %d, bit 0 is %d\n",i,j);
	syscon->API->Lock(syscon,DoRelease,0);
	return 1;
}
int enable_dio9(int command)
{
	Bus *syscon;
	syscon = BusInit1(0); // SYSCON Bus
	syscon->API->Lock(syscon,WaitLock,0);
	syscon->API->BitSet16(syscon,0x4,9);
	if(command == 1) 
	{
		syscon->API->BitSet16(syscon,0x6,9);
	}
	else
	{
		syscon->API->BitClear16(syscon,0x6,9);
	}
	syscon->API->Lock(syscon,DoRelease,0);
	return 1;
}
