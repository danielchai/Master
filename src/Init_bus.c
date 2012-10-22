/*************************************************************************
* Jianlong Chai  chai.4@wright.edu  June 6th 2012                        *
* This file contains two functions that are related to power management  *
* initbus() function used to power on/off (1/0) the 5V supply            *
* check_power() function used to check is 5V supply is on or off         *
**************************************************************************/
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

extern int login(char *event);
int initbus(int index)
{
   //Initialize the Bus for access other chip or memory 
	Bus *bus;
	bus = BusInit1(2);//use the MAX BUS 
	bus->API->Lock(bus,WaitLock,0);
	if(index == 1) 
	{
		bus->API->BitSet16(bus,2,12);//Turn on the Probotix power
		login("Enable probotixs power");
		printf("Enable power\n");

	}
	else 
	{
		bus->API->BitClear16(bus,2,12);
		login("Disable probotixs power");
		printf("Disable power\n");
	}
	bus->API->Lock(bus,DoRelease,0);
	return 1;
}
int check_power()
{
	int power_indicator;
	Bus *bus;
	bus = BusInit1(2);//use the MAX BUS 
	bus->API->Lock(bus,WaitLock,0);
	power_indicator = bus->API->BitGet16(bus,2,12);
	//int m = bus->API->BitGet16(bus,6,13);
	//printf("The AVR reset is %d\n",m);
	bus->API->Lock(bus,DoRelease,0);
	return power_indicator;
}

