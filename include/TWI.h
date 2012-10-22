#ifndef __TWI_H
#define __TWI_H

// TWIOpen modes
static const int TWIModeSlave = 0;
static const int TWIModeMaster = 1;

// TWIOpen error codes
static const int TWIErrorPortInvalid = -1;
static const int TWIErrorModeUnsupported = -2;
static const int TWIErrorBaudUnconstrained = -3;
static const int TWIErrorHardware = -4;

// TWIStart ops
static const int TWIOpWrite = 0;
static const int TWIOpRead = 1;

// TWI error codes for functions other than TWIOpen
static const int TWIErrorHandle =-1;
static const int TWIErrorWriteNAck = -2;
static const int TWIErrorDataNAck = -3;
static const int TWIErrorReadNack = -4;
static const int TWIErrorSDALow = -5;
static const int TWIErrorSCLLow = -6;
static const int TWIErrorAdrsLen = -7;
static const int TWIErrorTimeout = -8;
static const int TWIErrorOverrun = -9;
static const int TWIErrorArbLost = -10;
static const int TWIErrorUnknown = -999;
static const int TWIErrorOpInvalid = -7;

typedef struct TWI TWI;
typedef struct TWIAPI TWIAPI;

struct TWIAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  int (*Lock)(void *,int command,unsigned num);
  int (*Write)(void *,int devadr,int adrslen,int adrs,int len,char *bytes);
  int (*Read)(void *,int devadr,int adrslen,int adrs,int len,char *bytes);
};

struct TWI {
  TWIAPI *API;
  int InitStatus;
};

enum {
  NetTWI_Lock=0,
  NetTWI_Write=1,
  NetTWI_Read=2
};
#define TWI_APICount (1+NetTWI_Read)

#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
