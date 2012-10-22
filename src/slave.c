/*********************************************************
*  Jianlong Chai chai.4@wright.edu June 6th 2012         *
*  This file contains all the functions that related to  *
*  slave mode. It receives the raw request from master,  *
*  then forward it to SDL500W. After that get the reply  *
*  from SDL500W and forward it to master                 *
*********************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <modbus.h>
#include  <sys/types.h> 
#include  <sys/stat.h> 
#include  <ExternVars.h>
#include  <Defs.h>
#include  <string.h>
#include  <time.h>
#include  <mod_register.h>
struct SLEN;
extern SLEN * getsdl_condition(int *num, SLEN *bexamp,int created_time);
extern char *json_array_option(int num,SLEN *examp);
extern int set_time (int date[]);

modbus_t * raw_sdl_connection(modbus_t * sdl)
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

uint8_t *raw_data_command(modbus_t *sdl,int *re_size,int size,uint8_t *raw_req)
{
	//modbus_t *sdl;
	int req_length,re_try = 0;
	//int rc;
	uint8_t *rsp;
	//raw_req = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
	rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
/*sdl_wifi:
	sdl = modbus_new_tcp("169.254.114.25",502);
	//sdl = modbus_new_tcp("192.168.1.25",502);
	struct timeval old_response_timeout;
	struct timeval response_timeout;

	modbus_get_response_timeout(sdl, &old_response_timeout);

	response_timeout.tv_sec = 2;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{    
		if(re_try < 2){re_try++; sleep(2);goto sdl_wifi;}
		return NULL;
	}
	*/
	req_length = modbus_send_raw_request(sdl, raw_req, size*sizeof(uint8_t));
	//rc = modbus_receive_confirmation(sdl, rsp);
	*re_size = modbus_receive_confirmation(sdl, rsp);
	//printf("%s\n",rsp);
	//printf("%d\n",*re_size);
	modbus_close(sdl);
	modbus_free(sdl);
	return rsp;
}
uint8_t * sdl_read_buffsize(modbus_t *sdl,int *size)
{
	int resend=0,re_con=0;	
	int rc, size_per_record,num_parameter,i,j;
	unsigned long num_record, num_byte;
	//modbus_t *sdl;
	uint16_t * SDL_DATA_BUFF;
	uint8_t * rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
/*
reconnect:	
	sdl = modbus_new_tcp("169.254.114.25",502);

	struct timeval old_response_timeout;
	struct timeval response_timeout;

	modbus_get_response_timeout(sdl, &old_response_timeout);

	response_timeout.tv_sec = 2;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{   
		if(re_con < 3)
		{
			re_con++;
			sleep(2);
			goto reconnect;
		}
		else return NULL;
		
	}
*/
tryagain:
	SDL_DATA_BUFF = (uint16_t*)malloc(4*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,UT_REGISTERS_RD_DATA_BUFFSIZE_ADDRESS,2,SDL_DATA_BUFF);
	if (rc != 2) 
	{
		if(resend < 3)
		{
			resend++; 
			sleep(2);       		
			goto tryagain;
		}
		else return -100000;
	}
	i = 0;
	for(j=0;j<rc;j++)
	{
		rsp[i+1] = SDL_DATA_BUFF[j] & 0x00ff;
		rsp[i] = SDL_DATA_BUFF[j]>>8;		
		i = i+2;
	}
	*size = 2*rc;
	return rsp;
}
uint8_t *sdl_read_data(modbus_t *sdl,int *size,int register_num)
{
	
	int retry=0;	
	int rc,read_num,i,j;
	int created_time;
	float parameter;
	long long_value;
	uint16_t *SDL_Paramenters;	
	read_num = 2*register_num;
	//modbus_t *sdl;
	uint8_t * rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
/*	
	sdl = modbus_new_tcp("169.254.114.25",502);

	struct timeval old_response_timeout;
	struct timeval response_timeout;

	modbus_get_response_timeout(sdl, &old_response_timeout);

	response_timeout.tv_sec = 2;
	response_timeout.tv_usec = 0;
	modbus_set_response_timeout(sdl, &response_timeout);

    modbus_set_debug(sdl, TRUE);
	modbus_set_error_recovery(sdl,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
	
	modbus_set_slave(sdl,1);

	if (modbus_connect(sdl) == -1) 
	{   
		return NULL;
	}
*/
try_more:
	// The number 6 is the extra bytes that we need to store the data such as checksum
	SDL_Paramenters = (uint16_t*)malloc(read_num*sizeof(uint16_t));
	rc = modbus_read_input_registers(sdl,UT_REGISTERS_RD_DATA_BUFF_ADDRESS,read_num,SDL_Paramenters);
	//*size = modbus_receive_confirmation(sdl, rsp);
    if (rc != read_num) 
	{
		if(retry < 3)
		{
			retry++;   
			sleep(2);     		
			goto try_more;
		}
		else return -100000;
	}
	i = 0;
	for(j=0;j<rc;j++)
	{
		rsp[i+1] = SDL_Paramenters[j] & 0x00ff;
		rsp[i] = SDL_Paramenters[j]>>8;		
		i = i+2;
		//printf("%x %x\n",rsp[i+1],rsp[i]);
	}
	*size = 2*rc;
	return rsp;
}
int time_convert(int year,int month,int day,int hour,int minute,int second)
{
	printf("Year= %d Month= %d Day= %d Hour= %d Minute= %d Second = %d\n",year,month,day,hour,minute,second);	
	int num_records;
	time_t convert_time,now;
	struct tm timeend;
	char *return_value;
	timeend.tm_year = year-1900;
	timeend.tm_mon = month-1;
	timeend.tm_mday = day;
	timeend.tm_hour = hour;
	timeend.tm_min = minute;
	timeend.tm_sec = second;
	timeend.tm_isdst = -1;
	convert_time = mktime(&timeend);
	//printf("%d\n",convert_time);
	//printf("%s\n",ctime(&convert_time));		
	SLEN * sdl_records;
	sdl_records = (SLEN*)malloc(100*sizeof(SLEN));
	sdl_records = getsdl_condition(&num_records, sdl_records,convert_time);
	return_value = json_sdl_option(num_records,sdl_records);
	printf("%s\n",return_value);
	free(sdl_records);
	return convert_time;

}
int extend(unsigned int highbit, unsigned int lowbit)
{
	int longbits;
	longbits = (( highbit << 8 ) | 0x00ff) & ( lowbit | 0xff00 );	
	return longbits;
}

int slave()
{
    modbus_t *ctx;
	modbus_t *sdl;
    modbus_mapping_t *mb_mapping;
    int rc;
    uint8_t *query;
	uint8_t *raw_rsp;
	uint8_t *con_rsp;
	uint16_t *raw_rd_rsp;
	uint16_t *rd_rsp;
    int header_length;
	int year,month,day,hour,minute,second;
	int i,j;
	int rsp_length;
	int set_date[6];
	mb_mapping = modbus_mapping_new(
        UT_BITS_ADDRESS + UT_BITS_NB,
        UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,
        UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,
        UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);
    if (mb_mapping == NULL) 
	{
        modbus_free(ctx);
        return -1;
    }
    ctx = modbus_new_rtu("/dev/ttyS2", 9600, 'N', 8, 1);
    modbus_set_slave(ctx, 1);
    query = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
	raw_rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
	con_rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
	raw_rd_rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
	rd_rsp = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
    header_length = modbus_get_header_length(ctx);

    //modbus_set_debug(ctx, TRUE);
start:
    rc = modbus_connect(ctx);
    if (rc == -1) 
    {
		modbus_free(ctx);
		return -1;
    }
    for (;;) 
    {
        rc = modbus_receive(ctx, query);
		sdl = raw_sdl_connection(sdl);
		if(sdl == NULL)
		{
			modbus_free(sdl);
			return -1;
		}
        if (rc == -1) 
		{
            break;
   	    }
		//Send the raw message to SDL
		if(extend(query[2],query[3]) == 5152)
		{
			
			raw_rsp = raw_data_command(sdl,&rsp_length,rc,query);
			if(raw_rsp == NULL){printf("SDL is sleeping\n");goto close;}
			for(i=6;i<rsp_length;i++)
			{
				con_rsp[i-6]=raw_rsp[i];
			}
			modbus_send_raw_request(ctx, con_rsp, (rsp_length-6)*sizeof(uint8_t));
		}
		if(extend(query[2],query[3]) == 5168)
		{
			
			raw_rsp = raw_data_command(sdl,&rsp_length,rc,query);
			if(raw_rsp == NULL){printf("SDL is sleeping\n");goto close;}
			for(i=6;i<rsp_length;i++)
			{
				con_rsp[i-6]=raw_rsp[i];
			}
			modbus_send_raw_request(ctx, con_rsp, (rsp_length-6)*sizeof(uint8_t));
		}
		if(extend(query[2],query[3]) == 16384) 
		{	
			raw_rsp = sdl_read_buffsize(sdl,&rsp_length);
			if(raw_rsp == NULL){printf("SDL is sleeping\n");goto close;}
			for(i=0;i<2;i++)
			{
				con_rsp[i] = query[i];
			}
			con_rsp[2] = 0x4;
			for(i=0,j=3;i<rsp_length;i++)
			{
				con_rsp[j]=raw_rsp[i];
				j++;
			}
			modbus_send_raw_request(ctx, con_rsp, 7*sizeof(uint8_t));	
		}
		if(extend(query[2],query[3]) == 16386) 
		{	
			raw_rsp = sdl_read_data(sdl,&rsp_length,16);
			if(raw_rsp == NULL){printf("SDL is sleeping\n");goto close;}
			for(i=0;i<2;i++)
			{
				con_rsp[i] = query[i];
			}
			con_rsp[2] = 0x40;
			for(i=0,j=3;i<rsp_length;i++)
			{
				con_rsp[j]=raw_rsp[i];
				j++;
			}
			modbus_send_raw_request(ctx, con_rsp, 67*sizeof(uint8_t));	
		}
		
		if(extend(query[2],query[3]) == 1400) 
		{
			rc = modbus_reply(ctx, query, rc, mb_mapping);				
			set_date[0] = extend(query[7],query[8])+2000;
			set_date[1] = extend(query[9],query[10]);
			set_date[2]= extend(query[11],query[12]);
			set_date[3] = extend(query[13],query[14]);
			set_date[4] = extend(query[15],query[16]);
			set_date[5] = extend(query[17],query[18]);
			set_time(&set_date);
			//printf("%d %d %d %d %d %d\n",set_date[0],set_date[1],set_date[2],set_date[3],set_date[4],set_date[5]);
		}
		
    }
close:
	goto start;
    modbus_mapping_free(mb_mapping);
    free(query);
    modbus_free(ctx);
	

    return 0;
}
