/************************************************************
* Jianlong Chai chai.4@wright.edu  June 6th 2012            *
* All the functions here are used to control the SDL500W    *
* 1. Get the latest timestamp that in database              *
* 2. Extend 16 bits data to 32 bits                         *
* 3. Connect to SDL500W                                     *
* 4. Start profile command                                  *
* 5. Read sensor command                                    *
* 6. Set upload time command                                *
* 7. Set SDL500W clock command                              *
* 8. Next upload time command                               *
* 9. Read SDL buffer size command                           *
* 10. Upload data from SDL                                  *
************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <modbus.h>
#include  <sys/types.h> 
#include  <sys/stat.h> 
#include  <ExternVars.h>
#include  <Defs.h>
#include <time.h>
#include  <string.h>
#include <mod_register.h>
extern void enable(int command);
extern int * expected_time_get(int * expected_date);
extern ARF * getconfig(int *num, ARF *dexamp);
extern int login(char *event);
extern int setsdl(int time_stamp,double updatevalue);
extern int get_last_sdl_record();
struct ARF;
int * upload_time(int created_time)
{
	int *last_upload_time;
	last_upload_time = (int*)malloc(6*sizeof(int));
	time_t cal_time;
	struct tm * bro_time;
	cal_time = created_time;
	bro_time = gmtime(&cal_time);
    last_upload_time[0] = bro_time->tm_year-100;
	last_upload_time[1] = bro_time->tm_mon+1;
	last_upload_time[2] = bro_time->tm_mday;
	last_upload_time[3] = bro_time->tm_hour;
	last_upload_time[4] = bro_time->tm_min;
	last_upload_time[5] = bro_time->tm_sec;
	return last_upload_time;
}
long byte_extend(int highbit, int lowbit)
{
	int longbits;
	longbits = (( highbit << 16 ) | 0x0000ffff) & ( lowbit | 0xffff0000 );	
	return longbits;
}
modbus_t * sdl_connection(modbus_t * sdl)
{
	sdl = modbus_new_tcp("169.254.114.25",502);
	//sdl = modbus_new_tcp("192.168.1.25",502);
	struct timeval old_response_timeout;
	struct timeval response_timeout;

	/* Save original timeout */
	modbus_get_response_timeout(sdl, &old_response_timeout);

	/* Define a new and too short timeout! */
	response_timeout.tv_sec = 5;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    //modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{
	    return NULL;
	}
	login("Connect to sdl\n");
	return sdl;
}

/************************************************************************
* 		If the argument is greater than 2, then start profile. 			*
* 		num_position is the number of position ti profile.				*
* 	If the num_position is less than 2, then the number will be 0 or 1.	*
*		Means keep wifi ON/OFF during profiling                         *
************************************************************************/
int sdl_start_profile(modbus_t *sdl,unsigned int num_position)
{
	int resend=0;	
	int rc;
	//modbus_t *sdl;
	uint16_t *SDL_PROFILE_SET;
	SDL_PROFILE_SET = (uint16_t*)malloc(2*sizeof(uint16_t));
	SDL_PROFILE_SET[0]= num_position;
	SDL_PROFILE_SET[1] = 0x00;
try_again:
	rc = modbus_write_registers(sdl,UT_REGISTERS_START_PRO_ADDRESS,2,SDL_PROFILE_SET);
    if (rc != 2)
	{ 
		if(resend < 3)
		{
			sleep(2);
			resend++;
        	goto try_again;
		}
			else return -100000;
	}
	free(SDL_PROFILE_SET);
	return 1;
}
int sdl_test_profile(unsigned int num_position)
{
	int resend=0;	
	int rc;
	modbus_t *sdl;
	uint16_t *SDL_PROFILE_SET;
	SDL_PROFILE_SET = (uint16_t*)malloc(2*sizeof(uint16_t));
	SDL_PROFILE_SET[0]= num_position;
	SDL_PROFILE_SET[1] = 0x00;
try_again:
	rc = modbus_write_registers(sdl,UT_REGISTERS_START_PRO_ADDRESS,2,SDL_PROFILE_SET);
    if (rc != 2)
	{ 
		if(resend < 3)
		{
			sleep(2);
			resend++;
        	goto try_again;
		}
			else return -100000;
	}
	free(SDL_PROFILE_SET);
	return 1;
}
int sdl_read_sensor(modbus_t *sdl,unsigned int num_sample,unsigned int sample_interval)
{
	int resend=0;
	int rc;
	uint16_t  *SDL_READ_SENSOR_SET;
	SDL_READ_SENSOR_SET = (uint16_t*)malloc(3*sizeof(uint16_t));
	SDL_READ_SENSOR_SET[0] = num_sample;
	SDL_READ_SENSOR_SET[1] = 0;
	SDL_READ_SENSOR_SET[2] = sample_interval;
tryagain: 
	rc = modbus_write_registers(sdl,UT_REGISTERS_READ_SENSOR_ADDRESS,3,SDL_READ_SENSOR_SET);
	if(rc != 3)
	{
		if(resend < 3)
		{
			sleep(2);
			resend++;
			goto tryagain;
		}
		else return -100000;
	}
	free(SDL_READ_SENSOR_SET);
	return 1;
}
int sdl_setuploadtime(modbus_t *sdl,int year,int month,int day,int hour,int minute,int second)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_UPLOAD_TIME;
	SDL_UPLOAD_TIME = (uint16_t*)malloc(6*sizeof(uint16_t));
	SDL_UPLOAD_TIME[0] = year;
	SDL_UPLOAD_TIME[1] = month;
	SDL_UPLOAD_TIME[2] = day;
	SDL_UPLOAD_TIME[3] = hour;
	SDL_UPLOAD_TIME[4] = minute;
	SDL_UPLOAD_TIME[5] = second;
tryagain:
	rc = modbus_write_registers(sdl,UT_REGISTERS_UPLOAD_TIME_ADDRESS,6,SDL_UPLOAD_TIME);
	if(rc!=6)
	{
		if(resend < 3)
		{
			sleep(2);
			resend++;
        	goto tryagain;
		}
		else return -100000;
	}
	return 1;
}
int sdl_rtc_time(modbus_t *sdl,int year,int month,int day,int hour,int minute,int second)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_RTC_TIME;
	SDL_RTC_TIME = (uint16_t*)malloc(6*sizeof(uint16_t));
	SDL_RTC_TIME[0] = year;
	SDL_RTC_TIME[1] = month;
	SDL_RTC_TIME[2] = day;
	SDL_RTC_TIME[3] = hour;
	SDL_RTC_TIME[4] = minute;
	SDL_RTC_TIME[5] = second;
tryagain:
	rc = modbus_write_registers(sdl,UT_REGISTERS_SET_RTC_TIME,6,SDL_RTC_TIME);
	if(rc!=6)
	{
		if(resend < 3)
		{
			sleep(2);
			resend++;
        	goto tryagain;
		}
		else return -100000;
	}
	return 1;
}
/*********************************************************
*  This function is used to move to next data record     *
*  The special here is that there is no retry(According  *
*  to the profile document page 9) 						 *
*********************************************************/
int sdl_nextuploadtime(modbus_t *sdl)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_NEXT_UPLOAD;
	SDL_NEXT_UPLOAD = (uint16_t*)malloc(1*sizeof(uint16_t));
	SDL_NEXT_UPLOAD[0] = 1;

	rc = modbus_write_registers(sdl,UT_REGISTERS_NEXT_UPLOAD_TIME_ADDRESS,1,SDL_NEXT_UPLOAD);
	if(rc!=1)
	{
	 return -100000;
	}
	return 1;

}
int sdl_changewifi(modbus_t *sdl,unsigned int wifi_status)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_WIFI;
	SDL_WIFI = (uint16_t*)malloc(1*sizeof(uint16_t));
	SDL_WIFI[0] = wifi_status;
tryagain:
	rc = modbus_write_registers(sdl,UT_REGISTERS_CH_WIFI_ADDRESS,1,SDL_WIFI);
	if( rc!= 1)
	{
		if(resend <3 )
		{
			resend++;
			goto tryagain;
		}
		else 
			return -100000;
	}
	return 1;
}
int sdl_cancle_profile(modbus_t *sdl)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_CANCLE_PROFILE;
	SDL_CANCLE_PROFILE = (uint16_t*)malloc(1*sizeof(uint16_t));
	SDL_CANCLE_PROFILE[0] = 1;
tryagain:
	rc = modbus_write_registers(sdl,UT_REGISTERS_CANCLE_PROFILE_ADDRESS,1,SDL_CANCLE_PROFILE);
	if( rc!= 1)
	{
		if(resend <3 )
		{
			resend++;
			goto tryagain;
		}
		else 
			return -100000;
	}
	return 1;
}
int sdl_cancle_read_sensor(modbus_t *sdl)
{
	int resend=0;	
	int rc;
	uint16_t * SDL_CANCLE_READ_SENSOR;
	SDL_CANCLE_READ_SENSOR = (uint16_t*)malloc(1*sizeof(uint16_t));
	SDL_CANCLE_READ_SENSOR[0] = 2;
tryagain:
	rc = modbus_write_registers(sdl,UT_REGISTERS_CANCLE_PROFILE_ADDRESS,1,SDL_CANCLE_READ_SENSOR);
	if( rc!= 1)
	{
		if(resend <3 )
		{
			resend++;
			goto tryagain;
		}
		else 
			return -100000;
	}
	return 1;
}
/*******************************************************************************
* In this function, we need to read the registers to get the number of records *
* and the number od bytes													   *
********************************************************************************/
int sdl_readbuffsize(modbus_t *sdl)
{
	int resend=0;	
	int rc, size_per_record,num_parameter;
	unsigned long num_record, num_byte;
	uint16_t * SDL_DATA_BUFF;
tryagain:
	SDL_DATA_BUFF = (uint16_t*)malloc(4*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,UT_REGISTERS_RD_DATA_BUFFSIZE_ADDRESS,2,SDL_DATA_BUFF);
	if (rc != 2) 
	{
		if(resend < 3)
		{
			resend++;        		
			goto tryagain;
		}
		else return -100000;
	}
	num_record = SDL_DATA_BUFF[0];
	num_byte = SDL_DATA_BUFF[1];
	if((num_record == 0 )|(num_byte == 0))size_per_record = 0;
	else size_per_record = num_byte/num_record;
	printf("The size_per_record is %d\n",size_per_record);
	num_parameter = size_per_record/4;
	return num_parameter;
}
/*****************************************************************
* For this function, we need to know the number of the register. *
* Therefore, we can indicate how many registers we need to read  *
*****************************************************************/
int sdl_read_log_data(modbus_t *sdl,int register_num)
{
	
	int retry=0;	
	int rc,read_num;
	int created_time;
	float parameter;
	long long_value;
	uint16_t *SDL_Paramenters;	
	read_num = 2*register_num;
try_more:
	// The number 6 is the extra bytes that we need to store the data such as checksum
	SDL_Paramenters = (uint16_t*)malloc(read_num*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,UT_REGISTERS_RD_DATA_BUFF_ADDRESS,read_num,SDL_Paramenters);
    if (rc != read_num) 
	{
		if(retry < 3)
		{
			retry++;        		
			goto try_more;
		}
		else return -100000;
	}
	created_time = byte_extend(SDL_Paramenters[0],SDL_Paramenters[1]);
	int i;
	for(i=2;i<read_num-2;)
	{
		//printf("buff%d: %x%x\n",i,SDL_Paramenters[i],SDL_Paramenters[i+1]);
		long_value = byte_extend(SDL_Paramenters[i],SDL_Paramenters[i+1]);
		memcpy((void*)&parameter,(void*)&long_value,sizeof(float));
		//printf("parameter is %f\n",parameter);
		setsdl(created_time,parameter);
		i += 2;
	}
	return 1;
}
/*****************************************************************
* For this function, we need to know the number of the register. *
* Therefore, we can indicate how many registers we need to read  *
*****************************************************************/
int sdltestvalue()
{
	
	int retry=0;	
	int rc;
	modbus_t *sdl;
	uint16_t *SDL_TEST;	
	enable(1);
	sdl = modbus_new_tcp("169.254.114.25",502);

	struct timeval old_response_timeout;
	struct timeval response_timeout;

	/* Save original timeout */
	modbus_get_response_timeout(sdl, &old_response_timeout);

	/* Define a new and too short timeout! */
	response_timeout.tv_sec = 10;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{
	    return -100000;
	}
try_more:
	// The number 6 is the extra bytes that we need to store the data such as checksum
	SDL_TEST = (uint16_t*)malloc(2*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,0x1800,1,SDL_TEST);
    if (rc == 0) 
	{
	    return 0;
   	} 
	else 
	{
		if(retry < 3)
		{
			retry++;        		
			goto try_more;
		}
		else return -100000;
	}
	//modbus_close(sdl);
  	//modbus_free(sdl);
	return 1;
}
/*int sdl_read_position()
{
	int resend=0;	
	int rc, size_per_record;
	unsigned long num_record, num_byte;
	modbus_t *sdl;
	uint16_t * SDL_POSITION_BUFF;
	enable(1);
	sdl = modbus_new_tcp("169.254.114.25",502);

	struct timeval old_response_timeout;
	struct timeval response_timeout;

	modbus_get_response_timeout(sdl, &old_response_timeout);

	response_timeout.tv_sec = 5;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{
	    return -100000;
	}
tryagain:
	SDL_DATA_BUFF = (uint16_t*)malloc(4*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,UT_REGISTERS_RD_DATA_BUFFSIZE_ADDRESS,2, SDL_DATA_BUFF);
	if (rc != 2) 
	{
		if(resend < 3)
		{
			resend++;        		
			goto tryagain;
		}
		else return -100000;
	}
	num_record = SDL_DATA_BUFF[0];
	num_byte = SDL_DATA_BUFF[1];
	if((num_record == 0 )|(num_byte == 0))size_per_record = 0;
	else size_per_record = num_byte/num_record;
	printf("The size_per_record is %d\n",size_per_record);
	modbus_close(sdl);
  	modbus_free(sdl);
	return size_per_record;
}*/
int sample(unsigned int num_sample)
{
	int feedback = 0;	
	modbus_t *sdl;
	sdl = sdl_connection(sdl);
	if(sdl == NULL) 
	{
		login("Failed in taking with SDL500");
		exit(1);
	}
	feedback = sdl_start_profile(sdl,num_sample);
	if(feedback != -100000)return 1;
	else return -100000;

}
int profile_save_data(modbus_t *sdl)
{
	
	/*int num,year,month,day,hour,interval,num_record;
	ARF * configure;	
	unsigned int *load_time;
	load_time =  (int*)malloc(10*sizeof(int));
	configure = malloc(11*sizeof(ARF));
	load_time = expected_time_get(load_time);
	configure = getconfig(&num,configure);
	year = load_time[0] - 2000;
	month = load_time[1];
	day = load_time[2];
	hour = load_time[3] - configure[3].value;
	*/
	//Set the time stamp where start to upload
	unsigned int *load_time;
	int num_record;
	load_time =  (int*)malloc(6*sizeof(int));
	int created_time = get_last_sdl_record();
	load_time = upload_time(created_time);
	//printf("%d %d %d %d %d %d\n",load_time[0],load_time[1],load_time[2],load_time[3],load_time[4],load_time[5]);
	sdl_setuploadtime(sdl,load_time[0],load_time[1],load_time[2],load_time[3],load_time[4],load_time[5]+1);
readsize:	
	num_record = sdl_readbuffsize(sdl);
	if(num_record == 0) return 0;
	if(num_record == -100000)return -100000;
	//printf("%d\n",num_record);
	sdl_read_log_data(sdl,num_record);	
	sdl_nextuploadtime(sdl);
	goto readsize;
	return 1;
}

int sample_save_data(modbus_t *sdl)
{
	/*int num_record;	
	time_t rawtime;
	struct tm *timeinfo;
	int num,year,month,day,hour,interval;
	rawtime = time(NULL);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year -100;
	month = timeinfo->tm_mon +1;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	//printf("%d %d %d %d\n",year,month,day,hour);
	sdl_setuploadtime(sdl,year,month,day,hour,0,0);
	//sdl_setuploadtime(sdl,12,5,23,16,0,0);
readsize:	
	num_record = sdl_readbuffsize(sdl);
	if(num_record == 0) return 0;
	if(num_record == -100000)return -100000;
	//printf("%d\n",num_record);
	sdl_read_log_data(sdl,num_record);	
	return 1;
	*/
	unsigned int *load_time;
	int num_record;
	load_time =  (int*)malloc(6*sizeof(int));
	int created_time = get_last_sdl_record();
	load_time = upload_time(created_time);
	//printf("%d %d %d %d %d %d\n",load_time[0],load_time[1],load_time[2],load_time[3],load_time[4],load_time[5]);
	sdl_setuploadtime(sdl,load_time[0],load_time[1],load_time[2],load_time[3],load_time[4],load_time[5]+1);
readsize:	
	num_record = sdl_readbuffsize(sdl);
	if(num_record == 0) return 0;
	if(num_record == -100000)return -100000;
	//printf("%d\n",num_record);
	sdl_read_log_data(sdl,num_record);	
	sdl_nextuploadtime(sdl);
	goto readsize;
	return 1;
}
double sdltest(modbus_t *sdl)
{
	int rc;
	uint16_t *tab_reg;
	tab_reg = (uint16_t*)malloc(4*sizeof(uint16_t));
	tab_reg[0] = 0x00;
	rc =  modbus_read_input_registers(sdl, 0, 1, tab_reg);
	if(rc != 1)printf("error\n");
	//printf("%lx\n",tab_reg[0]);
	int volcode,voldig,volten,volhund;
	volcode = tab_reg[0];
	voldig = volcode/100;
	volten = volcode/10-voldig*10;
	volhund = volcode-(volcode/10)*10;
	char s[5]="";
	sprintf(s,"%d.%d%d",voldig,volten,volhund);
	double voltage =atof(s);
	return voltage;
}
double sdl()
{
	int rc;
	modbus_t *sdl;
	uint16_t tab_reg[4];
	tab_reg[0] = 0x00;
	enable(1);
	sdl = modbus_new_tcp("169.254.114.25",502);

	struct timeval old_response_timeout;
	struct timeval response_timeout;

	/* Save original timeout */
	modbus_get_response_timeout(sdl, &old_response_timeout);

	/* Define a new and too short timeout! */
	response_timeout.tv_sec = 5;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{
	    //fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
	    modbus_free(sdl);
	    return -100000;
	}

	rc =  modbus_read_input_registers(sdl, 0, 1, tab_reg);
	if(rc != 1)printf("error\n");
	printf("%lx\n",tab_reg[0]);
	int volcode,voldig,volten,volhund;
	volcode = tab_reg[0];
	voldig = volcode/100;
	volten = volcode/10-voldig*10;
	volhund = volcode-(volcode/10)*10;
	char s[5]="";
	sprintf(s,"%d.%d%d",voldig,volten,volhund);
	double voltage =atof(s);
  	modbus_close(sdl);
  	modbus_free(sdl);
	return voltage;
}
