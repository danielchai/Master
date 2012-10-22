/*************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012  *
*************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <modbus.h>
#include  <sys/types.h> 
#include  <sys/stat.h> 
#include  <ExternVars.h>
#include  <Defs.h>
#include  <string.h>
#include <mod_register.h>
/**********************************************************************************
* goup(modbus_t *ctx)                                                             *
* Use libmodbus function "modbus_write_registers" write value 0x02 to register 10 * 
* Ask the motor to go up, during this procedure, the home swicth can stop motor   *
**********************************************************************************/
extern int login( char *event);
int goup(modbus_t *ctx)
{
	int rc,resend=0;
tryagain:	
	UT_REGISTERS_TAB[0]=0x02;
	rc = modbus_write_registers(ctx, UT_REGISTERS10_ADDRESS,
                                	1, UT_REGISTERS_TAB);
	if (rc != 1) 
	{
		if(resend < 3) 
		{
			sleep(3);
			resend++;
			goto tryagain;
		} 
		else 
		{
			resend = 0;
			login("Go up command: failed");
			return -1;
		}
	}
	login("Go up command: succeed");
	return 1;
}

/*************************************************************************************
* godown(modbus_t *ctx)     														 *
* Use libmodbus function "modbus_write_registers" write value 0x01 to register 10    * 
* Ask the motor to go down, during this procedure, the home swicth can't stop motor  *
**************************************************************************************/
int godown(modbus_t *ctx)
{
	int rc,resend=0;
tryagain:
	UT_REGISTERS_TAB[0]=0x01;
	rc = modbus_write_registers(ctx, UT_REGISTERS10_ADDRESS,
                               1, UT_REGISTERS_TAB);
	if (rc != 1) 
	{
		if (resend < 3) 
		{
			sleep(3);
			resend++;
			goto tryagain;
		} 
		else 
		{
			resend = 0;
			login("Go down command: failed");
			return -1;
		}
	}
	login("Go down command: succeed");
	return 1;
}
/*************************************************************************************
* stop(modbus_t *ctx)     														     *
* Use libmodbus function "modbus_write_registers" write value 0x00 to register 10    * 
* Ask the motor to stop                                                             *
**************************************************************************************/
int stop(modbus_t *ctx)
{
	int rc,resend=0;
tryagain:
	UT_REGISTERS_TAB[0]=0x00;
	printf("1\n");
	rc = modbus_write_registers(ctx, UT_REGISTERS10_ADDRESS,
                                1, UT_REGISTERS_TAB);
	if (rc != 1) 
	{
		if(resend < 3) 
		{
			sleep(3);
			resend++;
			goto tryagain;
		} 
		else 
		{
			resend = 0;
			login("Stop command: failed");
			return -1;
		}
	}
	login("Stop command: succeed");
	return 1;
}
