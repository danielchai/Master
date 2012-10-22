#ifndef __DIO_H
#define __DIO_H
typedef enum { 
  INPUT_LOW=-3,INPUT_HIGH=-2,INPUT=-1,LOW=0, HIGH=1,
} DIOState;
static inline DIOState DIOValue(DIOState state) {
  switch (state) {
  case INPUT_LOW:
  case LOW:
    return LOW;
  case INPUT_HIGH:
  case HIGH:
    return HIGH;
  default: return state;
  }
}

static inline const char *DIOValueString(DIOState state) {
  switch(state) {
  case INPUT_LOW: return "InputLow";
  case INPUT_HIGH: return "InputHigh";
  case INPUT: return "Input";
  case LOW: return "Low";
  case HIGH: return "High";
  default: return "Invalid";
  }
}

typedef enum {
  DIO_NOT_PRESENT=0,
  DIO_CAN_INPUT=1,
  DIO_CAN_DRIVE_HIGH=2,
  DIO_CAN_DRIVE_LOW=4,
  DIO_INPUT_ONLY=1, // DIO_CAN_INPUT
  DIO_OUTPUT_ONLY=6, // (DIO_CAN_DRIVE_HIGH | DIO_CAN_DRIVE_LOW)
  DIO_NORMAL=7 // (DIO_CAN_DRIVE_HIGH | DIO_CAN_DRIVE_LOW | DIO_CAN_INPUT)
} DIOCaps;

typedef struct DIO DIO;
typedef struct DIOAPI DIOAPI;

struct DIOAPI {
  int (*Init)(void *);
  void (*Fini)(void *);
  int (*Lock)(void *,int command,unsigned num);
  void (*Refresh)(void *);
  void (*Commit)(void *,int forceall);
  void (*Set)(void *,int DIONum,DIOState State);
  DIOState (*Get)(void *,int DIONum);
  void (*SetAsync)(void *,int DIONum,DIOState State);
  DIOState (*GetAsync)(void *,int DIONum);
  int (*Wait)(void *,int *match,int min,int max,int nh,int *h,int nl,int *l);
  unsigned (*Count)(void *);
  DIOCaps (*Capabilities)(void *,unsigned num);
};

struct DIO {
  DIOAPI *API;
  int InitStatus;
};

enum {
  NetDIO_Lock=0, 
  NetDIO_Refresh=1, 
  NetDIO_Commit=2, 
  NetDIO_Set=3, 
  NetDIO_Get=4, 
  NetDIO_SetAsync=5,
  NetDIO_GetAsync=6,
  NetDIO_Wait=7,
  NetDIO_Count=8,
  NetDIO_Capabilities=9
};
#define DIO_APICount (1+NetDIO_Capabilities)

#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
