#include  <Python.h>
#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <modbus.h>
#include  <sys/types.h> 
#include  <sys/stat.h> 
#include  <getopt.h>
#include  <signal.h>
#include  <time.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <math.h>
#include  <ExternVars.h>
#include  <Defs.h>
#include  <Def_register.h>
#include  <mod_register.h>
#include  <ExternAPI.h>


char * program_name;
char *command_name;


void process_profile(int argsize, int arg1, int arg2, int arg3)
{
    PyObject *arg, *mymod, *strfunc;
    PyObject *pystatus;
    Py_Initialize();
    mymod = PyImport_ImportModule("python_send");
    strfunc = PyObject_GetAttrString(mymod,"exe_cmd_profile_op");
    arg = Py_BuildValue("(iiii)",argsize,arg1,arg2,arg3);
    pystatus = PyEval_CallObject(strfunc,arg);
    Py_Finalize();
}

void re_connect(modbus_t *ctx)
{
	enable(0);
	initbus(1);
	sleep(1);
	ctx = modbusconnection(ctx);
}
int wait_stop(int *ShmPTR,modbus_t *ctx, uint16_t *tab_rp_registers)
{
	int re_try=0,re_power=0;
	int stop_bit;
	long motor_status;
wait_stop:
			sleep(2); 
			//stop_bit = *ShmPTR;
			//if(stop_bit == 1)return 1;
			ctx = modbusconnection(ctx);
			motor_status = checkmotorstatus(ctx,tab_rp_registers);
			if(motor_status == -1)
			{
				if(re_power <= 3)
				{		
					if(re_try < 2 )
					{
						sleep(3);
						re_try++;
						goto wait_stop;
					}
					else
					{
						re_try = 0;
						enable(0);
						initbus(0);
						sleep(3);
						initbus(1);
						sleep(1);
						ctx = modbusconnection(ctx);
						re_power++;
						goto wait_stop;
					}
				}
				else
				{
					enable(0);
					initbus(0);
					enable(1);
					re_power = 0;
					return -1;
				}
			}
			if(motor_status != 0)goto wait_stop;
	return 1;
}
int wait_for_stop(modbus_t *ctx, uint16_t *tab_rp_registers)
{
	int re_try=0,re_power=0;
	int stop_bit;
	long motor_status;
wait_stop:
			sleep(2); 
			//stop_bit = *ShmPTR;
			//if(stop_bit == 1)return 1;
			ctx = modbusconnection(ctx);
			motor_status = checkmotorstatus(ctx,tab_rp_registers);
			if(motor_status == -1)
			{
				if(re_power < 1)
				{		
					if(re_try < 2 )
					{
						sleep(3);
						re_try++;
						goto wait_stop;
					}
					else
					{
						re_try = 0;
						enable(0);
						initbus(0);
						sleep(3);
						initbus(1);
						sleep(1);
						ctx = modbusconnection(ctx);
						re_power++;
						goto wait_stop;
					}
				}
				else
				{
					enable(0);
					initbus(0);
					enable(1);
					re_power = 0;
					return -1;
				}
			}
			if(motor_status != 0)goto wait_stop;
	return 1;
}
int schedule_reading()
{
	int *read_schedule,re_try = 0;
	long read_wait;
	modbus_t *sdl;
	double sdl_data;
	uint16_t *rd_data_registers;
    modbus_t *rd_ctx;
	read_schedule = (int*)malloc(6*sizeof(int));
	rd_data_registers = (uint16_t*)malloc(2*sizeof(uint16_t));
	read_schedule = check_time(read_schedule);
	if((read_schedule[4]>=3) && (read_schedule[4]<=10))
	{
		enable(0);
		initbus(1);
		sleep(1);
		rd_ctx= modbusconnection(rd_ctx);
		sleep(1);
		gotoposition(rd_ctx, 0,rd_data_registers);
		setconfig(9,0);
		wait_for_stop(rd_ctx,rd_data_registers);
		initbus(0);		
sdl_sample:
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			if(re_try < 3){re_try++;goto sdl_sample;}
			else 
			{
				login("Failed in connecting in sample duration");						
				setsdl(30000,-100000);
			}
		}
		else 
		{
			sdl_read_sensor(sdl,1,0);
			sleep(30);
			sample_save_data(sdl);
		}
		modbus_close(sdl);
	}
		return 1;
}

void print_comusage(FILE *stream, int exit_code)
{
	fprintf(stream, " Usage: %s command_name argument option <parameters> \n", program_name);
	fprintf(stream,
            "\tgo                      -d  --down           Run the winch down\n"
            "\t                        -u  --up             Run the winch up\n"
            "\t                        -p  <value>                              \n"
			"\t                               0             Run the winch go home\n"
			"\t                               position      Run the winch go to position\n"	
            "\tstop                                         Stop the winch\n"    
            "\tposition                                     Check the current position of winch\n"
			"\tstatus                  -m                   Check the status of the winch\n"
			"\t                        -S                   Check the status of the system\n"
			"\tcalibrate               -l  <value>          Set the position\n"
			"\t                        -c                   Count how many table space left for calibrate\n"							
			"\tprofile                 -d <value1> -s<value2> -w<value3> -i<value4>  Set the corresponding depth,spacing,dwell,interval\n"
            "\t                        -g                   After the setting of profile, then do profile\n"
			"\tprofileget                                   Get the profile parameters from database\n"
			"\tprofilego                                    Start profiling\n"
			"\treset                                        Reset the whole SQLite database\n"
			"\tinit                    all                  Initilize the whole SQLite tables\n"
			"\t                        caibrate             Initilize the whole calibration tables\n"
			"\t                        configure            Initilize the whole configure tables\n"
			"\tset_offset              -l<offset>           Set the offset value\n"
			"\tget_offset                                   Get the offset value\n"
			"\tbackup                                       It will use current database to set the factory default database\n"
			"\trestore                                      Reset the database to the factory default\n"
            "\tget                                          Get the calibrated data, returned in Json format\n"
            "\tslave                                        Ask the board go to modbus slave mode waiting for the data form host\n"
            "\tchecktime               This command will check the time and print the time use format:Today is  2012/2/14  Tuesday 11:17:7\n"
            "\tsettime                 date and time        Set the system's time,such as settime 2-12-2012 2:40:35\n"
			"\tenable_power                                 Enable the power to the probotixs driver board\n"
			"\tdisable_power                                Disable the power to the probotixs driver board\n"
			"\tpower_check                                  This command is used for check the motor board power\n"
			"\tsdl                                          Connect to SDL and read data from the SDL\n"
            "\tvoltage                                      This command allow the user to check the battery's voltage \n"
			"\tdebug                                        This command is used for debug purpose\n"
			"\tmotor_status                                 The purpose of this command is to check thr motor status and return non-Json format\n"
			"\tshutdown                                     It is used to shutdown the linux system, then user can power the system off safely\n"
			);
    exit(exit_code);   
}

int main(int argc, char *argv[])
{
	uint8_t *tab_rp_bits;
	uint16_t *tab_rp_registers;
	uint16_t *rd_position_registers;
    uint16_t *tab_rp_registers_bad;
    modbus_t *ctx;

/***********************************************************************
* feedback is used to store the return value of every called function
* rc is used to store the return value of the modbus command 
* resend is used to define the resend times if the command is failed
* i is used for the loop parameter
* insert_bit is used to indicate the calibarate function if there is value to set
* num is used to store the number of data blocks in database 
* use_backend is used to indicate the modbus mode is rtu
* next_option is used to take the command options
* pre_step, curr_step are used to indicate the previous step number and current position step number
* pre_length and value indicate the latest posiotion in database and current position
* SLEN is a kind of struct used to store the database blocks
************************************************************************/
    int feedback,i; 
	int insert_bit, nb_points,num =0;
	int next_option;
	long curr_step;
	double value;
	double pdepth,pspacing,pdwell,pinterval;
	double profiled,stopped;
	double depth,dwell,spacing,interval;
	double last_position;
	int profilebit =0,feedback1,feedback2;
	int modbus=0;
	int motor_stop = 0;
	char * command_arg = "";
	char * return_value;
	double in_position = 0;
	SLEN *examp;
	ARF  *config, *profile,*off_set;
	modbus_mapping_t *mb_mapping;
	int   ShmID;      
	int *ShmPTR;
	int stop_indicator = 0;

	key_t MyKey;
	MyKey   = ftok(".", 's');    
    ShmID   = shmget(MyKey, sizeof(int), IPC_CREAT | 0666);
    ShmPTR  = (int *) shmat(ShmID, NULL, 0);

	tab_rp_registers = (uint16_t *) malloc(4 * sizeof(uint16_t));
	rd_position_registers = (uint16_t *) malloc(2 * sizeof(uint16_t));
	tab_rp_registers_bad = (uint16_t *) malloc(2 * sizeof(uint16_t));

	config = (ARF*)malloc( 10 * sizeof(ARF) );
	if ( config == NULL ) 
	{
		printf("Error: Out of Memory, use ./master reset to reset memory\n"); 
		exit(1);
	}
	const char *const short_options = "hd::u::l:p::cD::w::s::i::gSmt::";
	const struct option long_options[] = {
        { "help",              0,NULL, 'h'},
        { "down",              2,NULL, 'd'},
		{ "up",                2,NULL, 'u'},
		{ "length",            1,NULL, 'l'},
		{ "position",          2,NULL, 'p'},
		{ "count",             0,NULL, 'c'},
		{ "Depth",             2,NULL, 'D'},
		{ "well",              2,NULL, 'w'},
		{ "spacing",           2,NULL, 's'},
		{ "interval",          2,NULL, 'i'},
		{ "go",                0,NULL, 'g'},
		{ "System",            0,NULL, 'S'},
		{ "motor",             0,NULL, 'm'},
		{ "time",              2,NULL, 't'},
        { NULL, 0, NULL, 0  },

    };

	if (argc < 2) 
	{
		print_comusage (stderr, 1);
		return_value = json_option("status:",-1);
		return return_value;
	}
	program_name = argv[0];
	/*Get the first argument that passed through main function but does not contain*/
	command_name = argv[1];
	if(argc > 2)
	{
		command_arg  = argv[2];
	}
/*******************************************************************************************
* The next three command_name are used to control the motor through modbus	(need modbus)  *
********************************************************************************************/

	if ( strcmp(command_name, "go") == 0 ) {
		double curr_position;
		char *recd = (char*)malloc(10*sizeof(char));
		double offset;
		int re_send = 0;
		*ShmPTR = 0;
		modbus = 1;
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		
		if (next_option == -1) print_comusage (stderr, 1);
		while (next_option != -1) 
		{
			switch (next_option) 
			{
				case 'h':
						print_comusage(stdout, 0);
				case 'd':  
				godown:
						enable(0);
						initbus(1);
						sleep(1);
						ctx = modbusconnection(ctx);
						modbus = 1;
						feedback = godown(ctx);
						if((feedback == -1)&&(re_send <1))
						{
							enable(0);
							initbus(0);
							re_send++;
							goto godown;
						}
						return_value = json_option("status",feedback);
						printf("%s\n",return_value);
						break;	
				case 'u':
				goup: 
						enable(0);
						initbus(1);
						sleep(1);
						ctx = modbusconnection(ctx);
						modbus = 1;
						feedback = goup(ctx);
						if((feedback == -1)&&(re_send <1))
						{
							enable(0);
							initbus(0);
							re_send++;
							goto goup;
						}
						return_value = json_option("status",feedback);
						printf("%s\n",return_value);
						break;	
				case 'p':
						enable(0);
						initbus(1);
						sleep(1);
						ctx = modbusconnection(ctx);
						modbus = 1;
						in_position = atof(optarg);		
						off_set = (ARF*)malloc(15*sizeof(ARF));
	    				off_set = getconfig(&num,off_set);
						offset = off_set[10].value;
						in_position = in_position - offset;
						free(off_set);
						//system("/home/sampler/kingkong.sh");
				gotoposition:
						if (in_position <= 0)gotoposition(ctx,0,rd_position_registers);
						else 
						{
							curr_position = checkposition(ctx,rd_position_registers);
							//printf("Position is %lf\n",curr_position);
							if ( !(( (in_position -0.1) <= curr_position ) && ( curr_position <= (in_position + 0.1) )) ) 
							{
								feedback = gotoposition(ctx, in_position,rd_position_registers);
								return_value = json_option("status",feedback);
								//printf("%s\n",return_value);
							}
						}
						if((feedback == -1)&&(re_send <1))
						{
							enable(0);
							initbus(0);
							enable(0);
							initbus(1);
							sleep(1);
							ctx = modbusconnection(ctx);
							re_send++;
						}
						break;	
					case '?':
						print_comusage (stderr, 1);
					default:
						abort ();
				}
			next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		}	
		//If the go command is failed, then exit the current process and power off the controller 
		if(feedback == -1)
		{
			*ShmPTR = 1;
			enable(0);
			initbus(0);
			return_value = json_option("status",-1);
			return return_value;
		}
		//wait_stop(ShmPTR,ctx,tab_rp_registers);
		do {
			sleep(1); 
			stop_indicator = *ShmPTR;              
		}while ((checkmotorstatus(ctx,tab_rp_registers)!= 0 ) && (stop_indicator != 1)); 
		sleep(1);
		//In order to avoid loop in checking position, get rid of using do-while   
		curr_position = checkposition(ctx,rd_position_registers);
		if(curr_position != -1)
		{
			login("Go to command set last position");
			curr_position = curr_position + offset;
			setconfig(9,curr_position);
			// In order to avoid "Read status command shutdown the power by accident"
			if( *ShmPTR == 0)
			{
				enable(0);
				initbus(0);	
				*ShmPTR = 1;
			}	
			return_value = json_option("status",1);
		}
		else return_value = json_option("status",-1);
	}

	if ( strcmp(command_name, "stop") == 0 ) 
	{ 		
		if(check_power() == 1)
		{
			sleep(1);
			ctx = modbusconnection(ctx);
			modbus = 1;
			feedback = stop(ctx);
			if(feedback == -1)stop(ctx);
		}
	}

    if ( strcmp(command_name, "position") == 0 ) 
	{ 
		login("Check position command");
		int resend = 0;
		double temp_position,offset;
		char *rec = (char*)malloc(10*sizeof(char));
		stop_indicator = *ShmPTR;
		uint16_t * position_registers = (uint16_t *) malloc(2 * sizeof(uint16_t));
		off_set = (ARF*)malloc(15*sizeof(ARF));
	    off_set = getconfig(&num,off_set);
		offset = off_set[10].value;
checkposition:
		if (check_power()== 1) 
		{
			ctx = modbusconnection(ctx);
			modbus = 1;
			temp_position = checkposition(ctx,position_registers);
			sprintf(rec,"The position read is %f",temp_position);
			login(rec);
			if(temp_position != -1)
			{
				login("Check position set last position");
				//This sentence is used to show the position with offset 
				temp_position = temp_position + offset;
				feedback = setconfig(9,temp_position);
			}
			else 
			{
				if(resend < 2)
				{
					resend++;					
					goto checkposition;
				}
				else return -100;
			}
		}
		else 
		{
			config = getconfig(&num,config);
			temp_position = config[8].value;
		}
		return_value = json_float_option("status",temp_position);
		printf("%s\n",return_value);
	}
/***********************************************************************
*         0: motor is stopped                                          *
*         1: motor is going down                                       *
*         2: motor is going up                                         *
*         3: motor is ramp up                                          *
*         4: motor is ramp down   									   *
*		   (need modbus)             						           *
*                                               	                   *
************************************************************************/
	if(strcmp(command_name, "status") == 0) 
	{
		stop_indicator = *ShmPTR;
		if (check_power()== 1) 
		{
			sleep(1);
			ctx = modbusconnection(ctx);
			modbus = 1;
			next_option = getopt_long (argc, argv, short_options, long_options, NULL);
			if(next_option == -1) print_comusage (stderr, 1);
			while(next_option != -1) 
			{
				switch (next_option) 
				{
					case 'h':
							print_comusage(stdout, 0);	
					case 'S':
							feedback = checksystemstatus(ctx,tab_rp_registers);
							return_value = json_option("status",feedback);
							break;	
					case 'm':
							feedback = checkmotorstatus(ctx,tab_rp_registers);
							return_value = json_option("status",feedback);
							break;
					case '?':
							print_comusage (stderr, 1);
					default:
							abort ();
				}
				next_option = getopt_long (argc, argv, short_options, long_options, NULL);
			}	
			if(feedback == -1)
			{
				return_value = json_option("status",0);
			}
		}
		else 
		{
			return_value = json_option("status",0);
			//login("Check status from database");
		}
		printf("%s\n",return_value);
	}

/****************************************************************************************
*      The next three command_name are used to control the database through sqlite3	    *
*****************************************************************************************/
	if ( strcmp(command_name, "factory_default") == 0 ) 
	{
		feedback1 = reset(0);
		feedback2 = dbinit(0);
		if ( (feedback1 == 1) && (feedback2 == 1)) 
		{
			return_value = json_float_option("status",1);
			printf("%s\n",return_value);
		}
		else 
		{
			return_value = json_float_option("status",-1);
			printf("%s\n",return_value);		
		}
	}

	if ( strcmp(command_name, "reset") == 0 ) 
	{
		feedback = reset(0);
		if(feedback == 1)
		{
			feedback = expected_time_reset();
		}
		return_value = json_float_option("status",feedback);
		printf("%s\n",return_value);
	}

	if ( strcmp(command_name, "init") == 0 ) 
	{
		feedback = -1;
		if ( strcmp(command_arg, "all") == 0 ) 
		{
			feedback = dbinit(0);
			if(feedback == 1)
			{
				feedback = expected_time_init();
			}
		}
		if ( strcmp(command_arg, "calibrate" ) == 0 ) 
		{ 
			setconfig(6,0);
			feedback = dbinit(1); 
		}
		if ( strcmp(command_arg, "configure" ) == 0 ) 
		{
			feedback = dbinit(2);
		}
		if ( feedback == -1 ) 
		{
			return_value = json_float_option("status",-1);
			print_comusage (stderr, 1);
		}
		else return_value = json_float_option("status",feedback);
		printf("%s\n",return_value);
	}

	if ( strcmp(command_name,"get") == 0 ) 
	{
		examp = getall(&num,examp);
		return_value = json_array_option(num,examp);
		free(examp);
		printf("%s",return_value);
	}
	if ( strcmp(command_name,"set_offset") == 0 ) 
	{
		double offset;		
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		if ( next_option == -1 ) print_comusage (stderr, 1);
		while( next_option != -1 ) 
		{
			switch (next_option) 
			{
				case 'h':
						print_comusage(stdout, 0);	
				case 'l':
						if(optarg!=0)offset = strtod(optarg,NULL);
						insert_bit = 1;
						break;	
				case '?':
						print_comusage (stderr, 1);
				default:
						abort ();
			}
			next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		}	
		feedback = setconfig(11,offset);
		return_value = json_option("status",feedback);
		printf("%s",return_value);
	}
	if ( strcmp(command_name,"get_offset") == 0 ) 
	{
		double offset;		
		off_set = (ARF*)malloc(15*sizeof(ARF));
	    off_set = getconfig(&num,off_set);
		offset = off_set[10].value;
		return_value = json_float_option("status",offset);
		printf("%s",return_value);	
		free(off_set);
	}
/**************************************************************************
*      The next three command_name are used to calibrate (need modbus)     *                     
***************************************************************************/
	if ( strcmp(command_name, "calibrate") == 0 ) 
	{	
		double calibrate;
		enable(0);
		initbus(1);
		sleep(1);
		ctx = modbusconnection(ctx);	
		modbus = 1;	
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		if ( next_option == -1 ) print_comusage (stderr, 1);
		config = getconfig(&num,config);
		calibrate = config[5].value;
		if ( calibrate == 0 ) 
		{
			reset(1);
			setconfig(6,1.0);
			set(num,0,0);
		}
		while( next_option != -1 ) 
		{
			switch (next_option) 
			{
				case 'h':
						print_comusage(stdout, 0);	
				case 'l':
						if(optarg!=0)value = atof(optarg);
						insert_bit = 1;
						break;	
				case 'c':
						getall(&num,examp);
						return_value = json_option("status",num);
						printf("%s\n",return_value);
						break;
				case '?':
						print_comusage (stderr, 1);
				default:
						abort ();
			}
			next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		}	
		if ( insert_bit == 1 ) 
		{
			curr_step = checksteps(ctx,rd_position_registers);
			if ( curr_step < 0 ) curr_step =0;//do not need
			feedback = checkvalue(curr_step,value);
			if ( feedback == 1 ) 
			{
				feedback = set(num,curr_step,value);
				return_value = json_option("status",feedback);
			} 
			else 
			{
				return_value = json_option("status",-1);
			}	
		}
		/*if ( checkmotorstatus(ctx,tab_rp_registers) == 0 ) 
		{	
			enable(0);
			initbus(0);
		}*/
		printf("%s\n",return_value);
	}


/***********************************************************************
*         The following functions are used for profile				   *
*                                               	                   *
************************************************************************/
	if ( strcmp(command_name, "profile") == 0 ) 
	{
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		if ( next_option == -1 ) print_comusage (stderr, 1);
		while ( next_option != -1 ) 
		{
			switch (next_option) {
				case 'h':
						print_comusage(stdout, 0);	
				case 'd':
						if(optarg!=0)depth = atof(optarg);
						profilebit = 1;
						break;	
				case 's':
						if(optarg!=0)spacing = atof(optarg);
						profilebit = 1;
						break;
				case 'w':
						if(optarg!=0)dwell = atof(optarg);
						profilebit = 1;
						break;
				case 'i':
						//if(optarg!=0)interval = atof(optarg);
						if(optarg!=0)interval = strtod(optarg,NULL);
						profilebit = 1;
						break;
				case '?':
						print_comusage (stderr, 1);
				default:
						abort ();
				}
			next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		}	

		if ( profilebit == 1 ) 
		{
			feedback = set_profile(depth,spacing,dwell,interval);
		}
		//Want to get the expected profile time and save it in database
		profile_time_check(interval);
		return_value = json_float_option("status",feedback);
		printf("%s\n",return_value);
	}

	if ( strcmp(command_name, "profileget" ) == 0) 
	{
		profile = getconfig(&num,config);
		return_value = json_profile_option(num-2,profile);
		free(profile);
		printf("%s",return_value);	
	}
	if ( strcmp(command_name, "profile_check" ) == 0) 
	{
		int *expected_profile_time;
		long remain_profile_time;
		config = getconfig(&num,config);
		pinterval = config[3].value;
		if(pinterval == 0)	
		{
			printf("-999\n");
			return -999;
		}
		expected_profile_time = (int*)malloc(10*sizeof(int));
		if(expected_profile_time == NULL){printf("error\n");exit(1);}
		expected_time_get(expected_profile_time);
		remain_profile_time = auto_run(0,expected_profile_time);
		if(remain_profile_time <=0 )remain_profile_time = 0;
		printf("%d\n",remain_profile_time);
		free(expected_profile_time);
		return remain_profile_time;
	}
	if ( strcmp(command_name, "profile_reset") == 0 ) 
	{
		//feedback = dbinit(2);
		//reading_hourly();
		system("ps aux | grep -e 'master profilego' | grep -v grep | awk '{print $2}' | xargs -i kill {}");
		feedback = set_profile(0,0,0,0);
		return_value = json_float_option("status",feedback);
		printf("%s\n",return_value);
	}

	if ( strcmp(command_name, "profilego") == 0 ) 
	{	
		double stayposition, curr_position,tmp,cal_position;
		double sdl_read,offset;
		long wait_time,motor_status;
		int year;
		time_t fail_time;
		modbus_t *sdl;
		int count,fini_count,re_try1 = 0,re_power1 =0,re_try = 0,re_send=0;
		int i=1,sample = 0,profile_times,sample_indicator;
		int * expected_tm, *curr_time,inter_val;
		*ShmPTR = 0;
		setconfig(10,0);
		//Will get the expected time from database and compare if it is roughly 
	profile:
		config = getconfig(&num,config);
		pdepth = config[0].value;
		pspacing = config[1].value;
		pdwell = config[2].value;
		pinterval = config[3].value;	
		profiled = config[4].value;
		sample = config[9].value;
		offset = config[10].value;
		profile_times = 1+(pdepth - offset)/pspacing;
		inter_val = (int)pinterval;
		if(pinterval == 0) exit(1);
	   	if(profiled == 0)
		{
			config = getconfig(&num,config);
			pdepth = config[0].value;
			pspacing = config[1].value;
			pdwell = config[2].value;
			pinterval = config[3].value;	
			profiled = config[4].value;
			sample = config[9].value;
			expected_tm = (int*)malloc(10*sizeof(int));
			curr_time = (int*)malloc(10*sizeof(int));
			if(curr_time == NULL){printf("error\n");exit(1);}
			if(expected_tm == NULL){printf("error\n");exit(1);}
			do{
				config = getconfig(&num,config);
				sample = config[9].value;
				expected_time_get(expected_tm);
				printf("Profile %d %d %d %d %d %d\n",expected_tm[0],expected_tm[1],expected_tm[2],expected_tm[3],expected_tm[4],expected_tm[5]);
				wait_time= auto_run(0,expected_tm);
				printf("Wait for %ds to profile\n",wait_time);
				curr_time = check_time(curr_time);
				printf("NOW %d %d %d %d %d %d\n",curr_time[0],curr_time[1],curr_time[2],curr_time[3],curr_time[4],curr_time[5]);
				sample_indicator = curr_time[3]%inter_val;
				//printf("%d\n",sample_indicator);
				if(wait_time < -600)
				{
					profile_time_check(pinterval);
					goto profile;
				}				
			}while(wait_time>0);
			printf("quit\n");
			free(expected_tm);	
			printf("free\n");
		four_minute_delay:
			sleep(1);
			curr_time = check_time(curr_time);
			if((curr_time[4]>=4) &&(curr_time[4]<=10))goto start_profile;
			if(curr_time[4]<4)goto four_minute_delay;
			if(curr_time[4]>10)goto profile;
		}
	start_profile:
		printf("start profile\n");
		if(profiled == 0)process_profile(3,1,profile_times,0);
		enable(0);
		initbus(1);
		sleep(1);
		ctx = modbusconnection(ctx);
		if(ctx == NULL)goto profile;
		modbus = 1;
		sleep(1);
		if ( profiled == 0 ) 
		{			
			printf("Start Profiling\n");
	        //Before go home, we do not need to check if it is home. 
			gotoposition(ctx, 0,rd_position_registers);
			wait_for_stop(ctx, tab_rp_registers);
			//Set the profile flag 
			setconfig(5,1);
			modbus_close(sdl);
			initbus(0);
			sleep(pdwell);
		}	
		printf("After go home\n");
		enable(0);
		printf("1\n");
		initbus(1);
		sleep(1);
		ctx = modbusconnection(ctx);
		//This following part is used to determine where is destination  
		curr_position = checkposition(ctx,tab_rp_registers) + offset;
		cal_position = i*pspacing + offset;
		if ( cal_position < pdepth )
		{
			stayposition = cal_position;
		}
		else 
			stayposition = pdepth;
		i++;
		stayposition = stayposition - offset;
		gotoposition(ctx,stayposition,tab_rp_registers);
		wait_for_stop(ctx, tab_rp_registers);
		//Here check position in order to determine if it is destination now 
		curr_position = checkposition(ctx,tab_rp_registers)+offset;
		printf("before ser database\n");
		setconfig(9,curr_position);
		printf("After set database\n");
		printf("Go to sleep for dwell time\n");
		initbus(0);
		sleep(pdwell);
		if (((pdepth -0.1) <= curr_position) && (curr_position <= (pdepth + 0.1))) 
		{ 
			
			setconfig(5,0);
			profile_time_check(pinterval);
			printf("2\n");
			enable(0);
			printf("3\n");
			initbus(1);
			ctx = modbusconnection(ctx);
			gotoposition(ctx, 0,rd_position_registers);
			wait_for_stop(ctx, tab_rp_registers);
			sleep(1);
			enable(0);
			printf("4\n");
			initbus(0);
			enable(1);
			//Save the position 0, set need_sleep to 0 and set stop indicate bit ShmPTR eqaul 1 
			printf("5before set datababase\n");
			setconfig(9,offset);
			//Read the SDL data and store it in database
			sleep(40);
		}
		goto profile;
	}
/***********************************************************************
*          The next three command_name are used                        *
*             to control the date and time         	                   *
************************************************************************/
	if ( strcmp(command_name, "checktime") == 0 ) 
	{
		/*char *sdate;
		if ( (sdate = malloc( 80 * sizeof(char) ) )== NULL)return NULL; 
		sdate = current_time(sdate);
		return_value = json_option_string("status",sdate);
		printf("%s\n",return_value);
		free(sdate);
		*/
		process_profile(3,1,3,1);
    }
	if ( strcmp(command_name, "settime") == 0 ) 
	{
    	if ( argc < 4 ) 
		{ 
			print_comusage (stderr, 1); 
		}
        char *date = argv[2];
        char *time = argv[3];
        int *buf = (int*)malloc(6*sizeof(int)); 
		parse(buf,date,time);
        int i,m_buf[6];
        for(i=0;i<=5;i++) 
		{ 
			m_buf[i]=*(buf+i); 
		}
		feedback = set_time(&m_buf);
        return_value = json_option("status:",feedback);
        printf("%s\n",return_value);
		login("Set local time");
		login(return_value);
		sleep(5);
	}
	if ( strcmp(command_name, "voltage") == 0 ) 
	{
		double voltage;	
		voltage = voltage_read();
		return_value = json_float_option("status",voltage);
		printf("%s\n",return_value);
	}
	if ( strcmp(command_name, "temp") == 0 ) 
	{
		double temp;
		temp = temp_read();
		return_value = json_float_option("status",temp);
		printf("%s\n",return_value);
	}
	if(strcmp(command_name, "enable_power") == 0)
	{
		enable(0);
		initbus(1);
		return_value = json_option("status:",1);
	}

	if(strcmp(command_name, "disable_power") == 0)
	{
		enable(0);
		initbus(0);
		return_value = json_option("status:",1);
	}
	if ( strcmp(command_name, "backup") == 0 ) 
	{
		feedback = system("cp /home/sampler/lr.sl3   /home/sampler/lr_default.sl3");
		if(feedback == -1)
		{
			return_value = json_float_option("status",-1);
		}
		else return_value = json_float_option("status",1);
		printf("%s\n",return_value);
	}
	if ( strcmp(command_name, "restore") == 0 ) 
	{
		feedback = system("cp /home/sampler/lr_default.sl3  /home/sampler/lr.sl3");
		if(feedback == -1)
		{
			return_value = json_float_option("status",-1);
		}
		else return_value = json_float_option("status",1);
		printf("%s\n",return_value);
	}
	if ( strcmp(command_name, "debug") == 0 ) 
	{
		long return_steps;
		char *debug_result;
		enable(0);
		initbus(1);
		sleep(1);
		ctx = modbusconnection(ctx);
		modbus = 1;
		uint16_t *debug_position_registers = (uint16_t*)malloc(2*sizeof(uint16_t));
		debug_result = (char*)malloc(50*sizeof(char));
		return_steps = checksteps(ctx,debug_position_registers);
		sprintf(debug_result,"%x,%x,%x\n",debug_position_registers[0],debug_position_registers[1],return_steps);
		json_option_string("status",debug_result);
		printf("%s\n",debug_result);
		initbus(0);
		
	}
	if ( strcmp(command_name, "power_check") == 0 ) 
	{
		int power_status = check_power();
		printf("Power is %d\n",power_status);
	}
	if ( strcmp(command_name, "sdl") == 0 ) 
	{
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl != NULL)
		{
			double vol = sdltest(sdl);
			return_value = json_float_option("status",vol);
			printf("%s\n",return_value);
			setsdl(30000,vol);
			login("Read SDL");
			login(return_value);
		}
		else setsdl(30000,-100000);
		modbus_close(sdl);
  		modbus_free(sdl);
	}
	if ( strcmp(command_name, "sdl_reset") == 0 ) 
	{
		resetsdl();
	}
	if ( strcmp(command_name, "sdl_get") == 0 ) 
	{
		int num_records;		
		SLEN * sdl_records;
		sdl_records = (SLEN*)malloc(100*sizeof(SLEN));
		sdl_records = getsdl(&num_records, sdl_records);
		return_value = json_sdl_option(num_records,sdl_records);
		printf("%s\n",return_value);
		free(sdl_records);
	}
	if ( strcmp(command_name, "sdl_uploadtime") == 0 ) 
	{	
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else sdl_setuploadtime(sdl,12,5,21,12,50,0);
	}
	if ( strcmp(command_name, "sdl_settime") == 0 ) 
	{	
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else  sdl_rtc_time(sdl,12,5,25,7,58,0);
	}
	if ( strcmp(command_name, "sdl_readsize") == 0 )
	{
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else sdl_readbuffsize(sdl);
	} 
	if ( strcmp(command_name, "sdl_readsensor") == 0 ) 
	{	
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else sdl_read_sensor(sdl,1,1);
	}
	if ( strcmp(command_name, "sdl_upload") == 0 ) 
	{
		//sdl_read_log_data(16);
		int number;
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else 
		{
			profile_save_data(sdl);
		}
		modbus_close(sdl);
	}
	if ( strcmp(command_name, "sdl_sample") == 0 ) 
	{
		int number;
		modbus_t * sdl;
		sdl = sdl_connection(sdl);	
		if(sdl == NULL)
		{
			setsdl(30000,-100000);
		}
		else 
		{
			sdl_read_sensor(sdl,1,1);
			sleep(60);
			sample_save_data(sdl);
			//sdl_start_profile(sdl,2);
		}
		modbus_close(sdl);
	}
	if ( strcmp(command_name, "shutdown") == 0 ) 
	{
		feedback = system("/sbin/shutdown now");
	}
	if ( strcmp(command_name, "maxstep") == 0 ) 
	{
		enable(0);
		initbus(1);		
		sleep(1);
		ctx = modbusconnection(ctx);
		modbus = 1;
		feedback = set_max_step(ctx,rd_position_registers);
		return_value = json_option("status",feedback);
		initbus(0);
	}
	if(strcmp(command_name, "slave") == 0)
	{
		slave();
	} 
	if(strcmp(command_name, "motor_status") == 0) 
	{
		if (check_power()== 1) 
		{
			sleep(1);
			ctx = modbusconnection(ctx);
			modbus = 1;
			feedback = checkmotorstatus(ctx,tab_rp_registers);
			if(feedback == -1)
			{
				printf("0\n");				
				return 0;
			}
			else 
			{
				printf("%d\n",feedback);
				return feedback;
			}
		}
		else
		{
			printf("0\n");
			return 0;
		}		
	}
close:
	/* Free the memory */
    //free(tab_rp_bits);
	free(config);
    free(tab_rp_registers);
	free(rd_position_registers);
	//modbus_mapping_free(mb_mapping);
    /* Close the connection */
	if (modbus == 1)	
	{
    	modbus_close(ctx);
	}

	if (motor_stop == 1) 
	{
		printf("stop setting\n");
		setconfig(9,last_position);
	}
    return return_value;
}
