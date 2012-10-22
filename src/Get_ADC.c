/**********************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012           *
* This part is used to control the ADC channel in ts8160  *
* This was used for detect voltage of battery. Now        *
* the function used to detect voltage is in Temp.c        *
***********************************************************/
#include <stdio.h>  
#include "json.h"
#include <assert.h>
#include <sys/mman.h>   
#include <stdlib.h>
#include <string.h>  
#include <sys/types.h> 
#include <unistd.h>
#include <sys/stat.h>   
#include <fcntl.h>     
#include "tsctllib.h"
#include "Bus.h"
#include <time.h>
#include "ExternVars.h"
#include "Defs.h"

volatile unsigned short *sysconregs;

static unsigned short peek16(unsigned int adr) {
	return sysconregs[adr / 2];
}

static void poke16(unsigned int adr, unsigned short val) {
	sysconregs[adr / 2] = val;
}
//Added 2-10-2012 5:12pm
float get_adc()
{
	int devmem,volcode,voldig,volten,volhund;
	devmem = open("/dev/mem", O_RDWR|O_SYNC);
	assert(devmem != -1);
	sysconregs = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, 
	  devmem, 0x30000000);
	poke16(0x80, 0x08); // configure ADC
	poke16(0x82, 0x10); // enable all 6 channels
	usleep(500000); // allow time for conversions
	volcode = (signed short)peek16(0x8e);
	volcode = (volcode * 1006)/200;//Because for the ts-8160 board, there is a circuit that provide input voltage input= realvoltage*(R2/(R1+R2)). 
                               //In this case, R1=200,R2=806.
	volcode = 2.1*(volcode * 2048)/0x8000;
	voldig = volcode/1000;
	volten = volcode/100-voldig*10;
	volhund = volcode/10-(volcode/100)*10;
	char s[5]="";
	sprintf(s,"%d.%d%d",voldig,volten,volhund);
	float m =atof(s);
	poke16(0x82, 0x00); // disable all 6 channels
    return m;
}
