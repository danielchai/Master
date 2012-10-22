/******************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012                   *
* These fucntions are related to rtu modbus command               *
* They are used to read steps, status, send go down, up           *
* go to, go home and stop command. Also contains function that    *
* extend 16 bits to 32 bits                                       *
*******************************************************************/
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
extern void enable(int command);
extern int login( char *event);
extern SLEN * getall(int *num, SLEN *bexamp);
int initbus(int index);

/****************************************************************
* @The register_8 and register_9 function are used to achieve:   *
* convert a 32 bits to 16 bits
****************************************************************/
unsigned int register_8(long value)
{
	unsigned int register8 = (value >>16) & (0x0000ffff);
	//printf("register_8 value: %#lx\n",register8);
	return register8;

}

unsigned int register_9(long value)
{
	unsigned int register9 = value & (0x0000ffff);
	//printf("register_9 value:%#lx\n",register9);
	return register9;

}
int set_max_step(modbus_t *ctx,uint16_t *rd_position_registers)
{
	int rc,resend=0;	
	uint16_t *Max_Step_Reg;
	Max_Step_Reg = (uint16_t*)malloc(2*sizeof(uint16_t));
tryagain:
	Max_Step_Reg[0] = 0x05;
	Max_Step_Reg[1] = 0x1a80;
	rc = modbus_write_registers(ctx,UT_REGISTERS20_ADDRESS,
                                	2, Max_Step_Reg);
	if (rc != 2) 
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
			return -1;
		}
	}
	return 1;
}
/********************************************************************
* @The extendbyte(unsigned int highbit, unsigned int lowbit)   		*
* combine two 16 bits to 32 bits---The return value will be steps,	*
* ,there is no float value.											*
********************************************************************/
long extendbyte(unsigned int highbit, unsigned int lowbit)
{
	long longbits;
	longbits = (( highbit << 16 ) | 0x0000ffff) & ( lowbit | 0xffff0000 );
	if (longbits <= 0)
	{
		longbits =0;
	}	
	//printf("32bits: %#lx\n",longbits);	
	return longbits;
}
/**********************************************************************
* long checksteps(modbus_t *ctx,uint16_t *rd_position_registers )     *
* Read the number of steps from the driver board					  *
**********************************************************************/
long checksteps(modbus_t *ctx,uint16_t *rd_position_registers)
{
	int rc,resend = 0;	
	login("Check steps involved");
tryagain:
	rc = modbus_read_input_registers(ctx,UT_INPUT_REGISTERS_ADDRESS,
                                     		2,
                                     	rd_position_registers);
	if (rc != 2) 
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
			login("Failed in reading steps from controller");
			return -1;
		}
	}
	//printf("Read Steps: %d,%d",rd_position_registers[0],rd_position_registers[1]);
	long rd_steps = extendbyte(rd_position_registers[0],rd_position_registers[1]);
	return rd_steps;
}
/****************************************************************
* checkposition(modbus_t *ctx,uint16_t *rd_position_registers)      *
* Convert the step information to position information			*
****************************************************************/
double checkposition(modbus_t *ctx,uint16_t *rd_position_registers)
{
	int i,num,try_time = 0;
	double position;
	SLEN *records;
	long steps = checksteps(ctx,rd_position_registers);
	char *loginfo = (char*)malloc(256*sizeof(char));
	sprintf(loginfo,"Steps read from controller is %d",steps);
	login(loginfo);
	if ( steps == -1 )
	{
		return -1;
	}
tryagain:
	records = getall(&num,records);
	
	if(num > 1)
	{
		if ( (steps >= records[num-1].step) )
		{
			//position = records[num-1].length;
			sprintf(loginfo,"Exceed maximum step in table. Read from controller is %d",steps);
			login(loginfo);
			position = -20;
		}
		else if ( (steps <= records[0].step) )
			 {
				position = 0;
			 }
			 else 
			 {
			 	for(i=0;i<num;i++)
				{
					if((steps >= records[i].step) && (steps <= records[i+1].step)) 
					{
						position = (records[i+1].length - records[i].length)*(steps - records[i].step)/(records[i+1].step - records[i].step)+records[i].length;
						break;
					}
			
				}	
			 }	
	}
	else 
	{
		if( try_time <= 5)
		{
			try_time++;	
			sleep(1);		
			goto tryagain;
			
		}
		else
			position = -10;
	}
	free(loginfo);
	//printf("CHANGE TO PISITION IS %lf\n",position);
	return position;
}
/****************************************************************
* positiontostep(double position)							    *
* Convert the position information to step information 			*
****************************************************************/
long positiontostep(double position)
{
	int i,num;
	long steps;
	SLEN *records;
	records = getall(&num, records);	
	if (num > 1) 
	{
		if(position >= records[num-1].length){steps = records[num-1].step;}
		else if ( position <= records[0].length )
			 {
			 	steps = 0;
			 }
			 else 
			 {
			  	for(i = 0;i < num;i++) 
				{
					if ( (position >= records[i].length) && (position <= records[i+1].length) ) 
					{
						steps = (records[i+1].step - records[i].step)*(position - records[i].length)/(records[i+1].length - records[i].length)+records[i].step;
					}
				}	
			 }	
	}
	else steps = 0;
	//printf("step: %d\n",steps);
   return steps;
}
/****************************************************************
* checkmotorstatus(modbus_t *ctx,uint16_t *tab_rp_registers )   *
* status: 0---------------Motor is stopped                      *
* status: 1---------------Motor is going down                   *
* status: 2---------------Motor is going up                     *
* status: 3---------------Motor is ramp up                      *
* status: 4---------------Motor is ramp down		            *
****************************************************************/
long checkmotorstatus(modbus_t *ctx,uint16_t *tab_rp_registers )
{
	int rc,resend =0;
	login("Check motor status involved");
tryagain:	
 	rc = modbus_read_input_registers(ctx, UT_REGISTERS12_ADDRESS,
                                     		1,
                                     	tab_rp_registers);
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
			login("Failed in reading motor status from controller");
			return -1;
		}
	}
	tab_rp_registers[1] = 0x00;
	long status = extendbyte(tab_rp_registers[1],tab_rp_registers[0]);
	//printf("Motor status: %#lx\n",status);
	return status;

}
/****************************************************************
* checksystemstatus(modbus_t *ctx,uint16_t *tab_rp_registers )  *
* status: 0---------------System no error						*
* status: 1---------------Home swicth not detected              *
* status: 2---------------Searching position                    *
****************************************************************/
long checksystemstatus(modbus_t *ctx,uint16_t *tab_rp_registers )
{
	int rc,resend=0;
	login("Check system status involved");
tryagain:
	 rc = modbus_read_input_registers(ctx, UT_REGISTERS13_ADDRESS,
                                     		1,
                                     	tab_rp_registers);
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
			login("Failed in read system status from controller");
			return -1;
		}
	}
	tab_rp_registers[1] = 0x00;
	long status = extendbyte(tab_rp_registers[1],tab_rp_registers[0]);
	//printf("System status: %#lx\n",status);
	return status;

}
/*********************************************************************************
* gotoposition(modbus_t *ctx, double in_position, uint16_t  * tab_rp_registers)  *
* Ask the motor to go to a specified position                                    *
**********************************************************************************/
int gotoposition(modbus_t *ctx, double in_position, uint16_t  * rd_position_registers)
{
	long arg,curr_step;
	int rc,resend=0;
	login("Goto command involved");	
	if(in_position < 0) in_position = 0;
tryagain:
	if (in_position != 0)
	{
		arg = positiontostep(in_position);
		printf("send to steps is %d\n",arg);
		//Because there is offset
		//curr_step = checksteps(ctx,tab_rp_registers);
		//if (arg > curr_step) arg = arg -Error_rec_factor;
		//if (arg < curr_step) arg = arg +Error_rec_factor;
		UT_REGISTERS_TAB[0]= register_8(arg);
		UT_REGISTERS_TAB[1]= register_9(arg);
	}
	else
	{
		UT_REGISTERS_TAB[0]= 0;
		UT_REGISTERS_TAB[1]= 0;
	}

	rc = modbus_write_registers(ctx, UT_REGISTERS8_ADDRESS,
                                	2, UT_REGISTERS_TAB);
	if (rc != 2) 
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
			login("Failed in sending goto command to controller");
			return -1;
		}
	}
	return 1;
}
/**********************************************************************************
* goup(modbus_t *ctx)                                                             *
* Use libmodbus function "modbus_write_registers" write value 0x02 to register 10 * 
* Ask the motor to go up, during this procedure, the home swicth can stop motor   *
**********************************************************************************/
int goup(modbus_t *ctx)
{
	int rc,resend=0;
	login("Goup command involved");
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
			login("Failed in sending goup command to controller");
			return -1;
		}
	}
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
	login("Godown command invovled");
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
			login("Failed in sending godown command to controller");
			return -1;
		}
	}
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
	login("Stop command invovled");
tryagain:
	UT_REGISTERS_TAB[0]=0x00;
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
			login("Failed in sending stop command to controller");
			return -1;
		}
	}
	return 1;
}
/*************************************************************************************
* checkvalue(long curr_step, double value)  										 *
* Before every calibrate setting, use this function to check the validation          *
*If the table is empty now, then insert the new data directly	return value is 1	 *
*If the table is full, then discard this new data  return value is 0                 * 
**************************************************************************************/
int checkvalue(long curr_step, double value)
{
	int i, num;
	SLEN *check;
	check = getall(&num, check);
	if(num == 0) 
	{
		return 1;
	}
	if(num >= Max_Records) 
	{ 
		return 0;
	}
	for(i=0;i<num;i++) 
	{
		if((curr_step > check[i].step) && (curr_step < check[i+1].step) && (value > check[i].length) && (value < check[i+1].length))break;
	}
	//printf("%d\n",i);
	if( i < num ) 
	{	
		return 1;
	}
	else if ((i == num) && (curr_step > check[num-1].step) && (value > check[num-1].length))
		 {
			return 1;
		 }
	     else 
		 {
			return -1;
		 }
}
/*******************************************
* modbus_t * modbusconnection(ctx)			   *
* Used to establish modbus connection      *
*******************************************/
modbus_t * modbusconnection(modbus_t * ctx)
{
	int error,reconnect=1;
	//modbus_t *ctx;
	enable(1);
	ctx = modbus_new_rtu("/dev/ttyS1", 19200, 'N', 8, 1);

	if (ctx == NULL) {
		return NULL;
    }
   	//modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	

   modbus_set_slave(ctx, SERVER_ID);

	do{
		error = modbus_connect(ctx);
		reconnect++;
		}while((error == -1)&&(reconnect <=3));
	if ( error == -1 ) { 
		//modbus_free(ctx); 
		return NULL;
		}
	return ctx;
}
