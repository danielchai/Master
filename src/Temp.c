/*******************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012        *
* This function is used to read tempeature from the    *
* off_board temperature sensor and read voltage from   *
* the power supply.                                    *
********************************************************/
#include <stdio.h>  
#include <tsctllib.h>
#include <time.h>
#include <unistd.h>
#include <ExternVars.h>
#include <Defs.h>
double temp_read()
{
	char buf[4];
	int OptRetries=7,ret;
	int sleep_time,mask;
	TWI *TWI_0;
	TWI_0 = TWIInit1(0);
	if (TWI_0->InitStatus <= 0)  {
    printf("error=\"Error %d initializing TWI\"\n",TWI_0->InitStatus);
    return -1;
	}
	int val;
  OptTempBegin:
    ret = TWI_0->API->Read(TWI_0,TEMP_ADDR,1,0x07,2,buf);
    if (ret < 0) {
      printf("error=\"TWI Read error %d\"\n",ret);
      return 3;
    }

    if (buf[0] != 0x01 || buf[1] != 0x90) {
      if (OptRetries-- > 0) goto OptTempBegin;
      printf("tempSensor=0\n");
      printf("Temp Sensor sanity check failed, got ");
      printf("%02X %02X\n",buf[0],buf[1]);
      return 1;
    }
    usleep(150000);
    buf[0] = (0x4001 | (1 << 15)) >> 8;
    buf[1] = (0x4001 | (1 << 15)) & 0xFF;
    TWI_0->API->Write(TWI_0,TEMP_ADDR,1,0x01,2,buf);

    usleep(150000);
    buf[0] = 0x40;
    buf[1] = 0x01;
    TWI_0->API->Write(TWI_0,TEMP_ADDR,0,0,2,buf);
    usleep(150000);
    TWI_0->API->Read(TWI_0,TEMP_ADDR,1,0x00,2,buf);

    //printf("tempSensor=1\n");

    val = (256*buf[0] + buf[1]) * 1000 / 128;
	double temperature = val/1000.00;
	//printf("The double format is %f\n",temperature);
    //printf("tempC=%d.%03d\n",val/1000,val%1000);
    val = val * 9 / 5 + 32000;
   // printf("tempF=%d.%03d\n",val/1000,val%1000);
	return temperature;
  }
double voltage_read()
{
	int OptInfo = 0,i,v;
	int ii=0,ij=0;
	double voltage;
	char buf[4];
	int OptRetries=7,ret;
	int sleep_time,mask;
	TWI *TWI_0;
	TWI_0 = TWIInit1(0);
	if (TWI_0->InitStatus <= 0)  {
    printf("error=\"Error %d initializing TWI\"\n",TWI_0->InitStatus);
    return -1;
	}
    if (OptInfo > 0) fprintf(stderr,"\r%d(%d) ",ii,ij);
    ii++;
  OptInfoBegin:
    ij++;
    ret = TWI_0->API->Read(TWI_0,AVR_ADDR,1,0x40,4,buf);
    if (ret < 0) {
      if (OptRetries-- > 0) goto OptInfoBegin;
      fprintf(stderr," error=\"TWI Read error %d\"\n",ret);
      //return 3;
    }
    if (buf[0] != 0x55) {
      if (OptRetries-- > 0) goto OptInfoBegin;
      fprintf(stderr,"Sanity check failed: read back ");
      for (i=0;i<4;i++) {
	fprintf(stderr,"%02X ",buf[i]);
      }
      fprintf(stderr,"\n");
      //printf("\nerror=\"sanity check failed\"\n");
    } else {
      if (OptInfo == 0 || buf[1] != 2) {
	//printf("avr_sw_rev=%d\n",buf[1]);
      }
      v = (unsigned)buf[2] * 256 + buf[3];
      v = v * 3690 / 4096;
      if (v < 300 || v > 4200) { // voltage way out of range, probably a TWI error, try again
	if (OptRetries-- > 0) goto OptInfoBegin;
      }
      // 1116 * REF / 4096 = 10.05
      if (OptInfo == 0 || v > 550 || v < 450) {
	//printf("voltage=%d.%02d\n",v / 100,v % 100);
	voltage = v/100.00;
	//printf("Double format voltage is %f\n",voltage);
	//printf("voltage_mv=%d\n",v*10);
	return voltage;
      }
    }



}
