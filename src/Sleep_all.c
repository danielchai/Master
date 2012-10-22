#include <stdio.h>  
#include <tsctllib.h>
#include <time.h>
#include <unistd.h>
#include <ExternVars.h>
#include <Defs.h>

extern int *check_time();
extern int setsystem(int index,int updatevalue);
extern int login(char *event);
/*
 The sleep_allmode is used to make the ts8160,ts4200 and probotix to sleep.
 It is achieved through driving EN_BYPASS and EN_BUCK pins on AVR low to go into Sleep mode
*/
int sleep_allmode(int sec)
{
	login("Go to sleep");
	char buf[4];
	int OptRetries=7,ret;
	int sleep_time,mask;
	
	//int *date;
	//int year,month,day,hour,minute,second;
	TWI *TWI_0;
	TWI_0 = TWIInit1(0);
	if (TWI_0->InitStatus <= 0)  {
    printf("error=\"Error %d initializing TWI\"\n",TWI_0->InitStatus);
    return -1;
	}
    mask = 0;
    
    if (sec < 0) 
	{
      sleep_time = -1;
    } 
	else if (sec < 262136 && ((sec % 2048) != 0)) 
		 {
      		if (sec % 8 != 0) sec += (8 - sec % 8);
      			sleep_time = sec / 8;
      			fprintf(stderr,"Sleeping for %d seconds\n",sec);
    	 } 
			else if (sec <= 67106816) 
				 {
      			 	if (sec % 2048 != 0) sec += (2048 - sec % 2048);
      				sleep_time = 0x8000 + sec / 2048;
      				fprintf(stderr,"Sleeping for %d seconds\n",sec);
    			 } 
				else 
				{
      				fprintf(stderr,"NOT sleeping.  Maximum sleep time is 67106816 seconds\n");
      				return -1;
    			}
    fprintf(stderr,"Reset switch will %s\n",!(mask&0x80)?"wake up":"be ignored");
    buf[0] = SLEEP;
    buf[1] = (sleep_time & 0xFFFF)>>8;
    buf[2] = (sleep_time & 0xFF);
    buf[3] = (mask & 0xFF);
OptSleepBegin:
    ret = TWI_0->API->Write(TWI_0,AVR_ADDR,0,0,4,buf);
    if (ret < 0) 
	{
    	if (OptRetries-- > 0) goto OptSleepBegin;
      	printf("error=\"TWI Write error %d\"\n",ret);
      	return -1;
    }
	printf("The system is going to deep sleep mode\n");
    usleep(100000); // wait for power loss
    if (OptRetries-- > 0) 
	{
		printf("Not sleep mode\n");
		goto OptSleepBegin; 
	}// we didn't sleep, try again
    TWI_0->API->Fini(TWI_0);
    return 1;
}
