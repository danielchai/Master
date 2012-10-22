#ifndef __ExternVars_H
#define __ExternVars_H
#include <DIO.h>
#include <signal.h>
#include <modbus.h>
//extern DIO *dio;
extern char * program_name;
extern char *command_name;
extern int accelerate,decelerate;
typedef struct Test{
        int id;
        long step;
		double length;
} SLEN ;

typedef struct Rest{
        int id;
        char * property;
		double value;
} ARF;
typedef struct System{
        int id;
        char * property;
		int value;
} System;

//extern sig_atomic_t keep_going = 0;

#endif
