#ifndef __CAN_H
#define __CAN_H
#include "Lock.h"

enum {
  CANEventBusError=0x80,
  CANEventArbLost=0x40, 
  CANEventErrorPassive=0x20,
  CANEventWakeUp=0x10,
  CANEventDataOverrun=8,
  CANEventErrorWarning=4,
  CANEventTransmitDone=2,
  CANEventReceiveReady=1
};

#define CAN_MSG_LENGTH 8
#define FLAG_BUS_ERROR 1
#define FLAG_ERROR_PASSIVE 2
#define FLAG_DATA_OVERRUN 4
#define FLAG_ERROR_WARNING 8
#define FLAG_RTR 0x10
#define FLAG_EXT_ID 0x20
#define FLAG_LOCAL 0x40
#define FLAG_CONTROL 0x80
#define FLAG_CMD_RXEN 1
/*
  Raw Packet Format:

  UINT8    flags:
               control information present (reserved for future use)
               message originates from this node (unused)
               message has extended ID
               remote transmission request (RTR)
               error warning
               data overrun
               error passive
               bus error
  UINT32   id
  UINT32   timestamp_seconds
  UINT32   timestamp_microseconds
  UINT8    bytes
  UINT8[8] data
 */
#include <sys/time.h>
#include <sys/select.h>

struct CANMessage {
  unsigned flags;  // flags
  unsigned id;
  struct timeval timestamp;
  unsigned length;
  unsigned char  data[CAN_MSG_LENGTH];
};
typedef struct CANMessage CANMessage;

typedef struct CAN CAN;
typedef struct CANAPI CANAPI;

/*
  Statistics:
  Tx count
  Rx count
  overruns
  other error counters
 */
struct CANAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  int (*Rx)(void *,CANMessage *);
  int (*Tx)(void *,CANMessage *);
  unsigned (*BaudSet)(void *,unsigned opt_baud);
  unsigned (*BaudGet)(void *);
};

struct CAN {
  CANAPI *API;
  int InitStatus;
  unsigned LockBase;
  int deferlock;
  int irq;
  int CAN_TX,CAN_RX;
};

enum {
  NetCAN_Rx=0, NetCAN_Tx=1, NetCAN_BaudSet=2, NetCAN_BaudGet=3
};
#define CAN_APICount (1+NetCAN_BaudGet)

#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
