#ifndef __ExternAPI_H
#define __ExternAPI_H
/*extern struct SLEN;
extern struct ARF;
extern struct System;
*/

extern int reset(int index);
extern int dbinit(int dbindex);
extern int setsystem(int index,int updatevalue);
extern System * getsystem(int *num);
extern SLEN * getall(int *num, SLEN *bexamp);
extern int set(int index,int steps,double value);
extern ARF * getconfig(int *num, ARF *dexamp);
extern int setconfig(int index,double updatevalue);



extern const char *json_option(char * property , int value);
extern const char *json_array_option(int num,SLEN *examp);
extern const char *json_float_option(char * property , float value);
extern const char *json_profile_option(int num,ARF *examp);
extern const char *json_option_string(char * property , char *content);
extern const char *json_sdl_option(int num,ARF *examp);
extern int set_profile(double depth, double spacing, double dwell, double interval);

extern int *check_time(int*date);
extern int set_time (int date[]);
extern char *current_time(char *sdate);

//xtern int * parse(char *date,char *time);
extern int * parse(int *return_date,char *date,char *time);
extern int initbus(int index);
extern float get_adc();
extern void enable(int command);
extern int sleep_allmode(int sec);
//extern int slave(char *argc);

extern modbus_t * sdl_connection(modbus_t * sdl);
double sdltest(modbus_t *sdl);
extern int sdl_start_profile(modbus_t *sdl,unsigned int num_position);
extern int sdl_read_sensor(modbus_t*sdl,int num_sample,int sample_interval);
extern int sdl_setuploadtime(modbus_t*sdl,int year,int month,int day,int hour,int minute,int second);
extern int sdl_nextuploadtime(modbus_t*sdl);
extern int sdl_changewifi(modbus_t*sdl,unsigned int wifi_status);
extern int sdl_cancle_profile(modbus_t*sdl);
extern int sdl_cancle_read_sensor(modbus_t*sdl);
extern int sdl_readbuffsize(modbus_t*sdl);
extern int sdl_read_log_data(modbus_t*sdl,int paremater_num);
extern int profile_save_data(modbus_t*sdl);
extern int sample_save_data(modbus_t *sdl);
extern double sdl(modbus_t*sdl);
extern int sdltestvalue();
extern int sdl_test_profile(unsigned int num_position);
extern int sdl_rtc_time(modbus_t *sdl,int year,int month,int day,int hour,int minute,int second);



extern int login(char * event);
extern int killprocess();
extern int check_power();

extern modbus_t * modbusconnection(modbus_t * ctx);
extern int goup(modbus_t *ctx);
extern int godown(modbus_t *ctx);
extern long checkmotorstatus(modbus_t *ctx,uint16_t *tab_rp_registers );
extern long checksystemstatus(modbus_t *ctx,uint16_t *tab_rp_registers );
extern int stop(modbus_t *ctx);
extern int checkvalue(long curr_step, double value);
extern long auto_run(int index, int *date);
extern unsigned int register_8(long value);
extern unsigned int register_9(long value);
extern long extendbyte(unsigned int highbit, unsigned int lowbit);
extern long checksteps(modbus_t *ctx,uint16_t *tab_rp_registers);
extern double checkposition(modbus_t *ctx,uint16_t *tab_rp_registers);
extern long profile_time_check(double pinterval);
extern long positiontostep(double position);
extern int gotoposition(modbus_t *ctx, double in_position, uint16_t  * tab_rp_registers);
extern int set_max_step(modbus_t *ctx,uint16_t *rd_position_registers);


extern int expected_time_reset();
extern int expected_time_init();
extern int expected_time_set(int *expected_date);
extern int * expected_time_get(int * expected_date);
//extern int winch_sleep(int second);
extern long reading_hourly();
extern int enable_wakeup(int command);
extern int enable_dio9(int command);
extern int resetsdl();
extern int setsdl(int time_stamp,double updatevalue);
extern SLEN * getsdl(int *num, SLEN *bexamp);

extern int get_count();
extern int slave();
extern double temp_read();
extern double voltage_read();
extern SLEN * getsdl_condition(int *num, SLEN *bexamp,int created_time);
/*
extern int sdl_read_sensor(modbus_t *sdl,int num_sample,int sample_interval);
extern int sdl_setuploadtime(modbus_t *sdl,int year,int month,int day,int hour,int minute,int second);
extern int sdl_nextuploadtime(modbus_t *sdl);
extern int sdl_changewifi(modbus_t *sdl,unsigned int wifi_status);
extern int sdl_readbuffsize(modbus_t *sdl);
extern int sdl_read_log_data(modbus_t *sdl,int paremater_num);
extern int profile_save_data(modbus_t *sdl);
*/
//extern int * expected_time_get();
#endif

