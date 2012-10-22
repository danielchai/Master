#ifndef __Bus_H
#define __Bus_H
#include "Lock.h"

typedef struct Bus Bus;
typedef struct BusAPI BusAPI;

struct BusAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  int (*Lock)(void *,int command,unsigned num);
  unsigned char (*Peek8)(void *,int adrs);
  void (*Poke8)(void *,int adrs,unsigned char val);
  unsigned short (*Peek16)(void *,int adrs);
  void (*Poke16)(void *,int adrs,unsigned short val);
  unsigned (*Peek32)(void *,int adrs);
  void (*Poke32)(void *,int adrs,unsigned val);
  int (*BitGet8)(void *bus,int adrs,int bit);
  void (*BitAssign8)(void *bus,int adrs,int bit,int val);
  void (*BitSet8)(void *bus,int adrs,int bit);
  void (*BitClear8)(void *bus,int adrs,int bit);
  int (*BitGet16)(void *bus,int adrs,int bit);
  void (*BitAssign16)(void *bus,int adrs,int bit,int val);
  void (*BitSet16)(void *bus,int adrs,int bit);
  void (*BitClear16)(void *bus,int adrs,int bit);
  int (*BitGet32)(void *bus,int adrs,int bit);
  void (*BitAssign32)(void *bus,int adrs,int bit,int val);
  void (*BitSet32)(void *bus,int adrs,int bit);
  void (*BitClear32)(void *bus,int adrs,int bit);
  void (*PeekStream)(void *bus,int adr,int dir,char *buf,int len);
  void (*PokeStream)(void *bus,int adr,int dir,char *buf,int len);
  void (*Refresh)(void *);
  void (*Commit)(void *,int forceall);
  int (*BitToggle8)(void *bus,int adrs,int bit);
  int (*BitToggle16)(void *bus,int adrs,int bit);
  int (*BitToggle32)(void *bus,int adrs,int bit);
};

enum {
  NetBus_Lock=0,
  NetBus_Peek8=1,
  NetBus_Poke8=2,
  NetBus_Peek16=3,
  NetBus_Poke16=4,
  NetBus_Peek32=5,
  NetBus_Poke32=6,
  NetBus_BitGet8=7,
  NetBus_BitAssign8=8,
  NetBus_BitSet8=9,
  NetBus_BitClear8=10,
  NetBus_BitGet16=11,
  NetBus_BitAssign16=12,
  NetBus_BitSet16=13,
  NetBus_BitClear16=14,
  NetBus_BitGet32=15,
  NetBus_BitAssign32=16,
  NetBus_BitSet32=17,
  NetBus_BitClear32=18,
  NetBus_PeekStream=19,
  NetBus_PokeStream=20,
  NetBus_Refresh=21,
  NetBus_Commit=22,
  NetBus_BitToggle8=23,
  NetBus_BitToggle16=24,
  NetBus_BitToggle32=25
};

#define Bus_APICount (1+NetBus_BitToggle32)
struct Bus {
  BusAPI *API;
  int InitStatus;
  unsigned LockBase;
  int deferlock;
};
// __attribute__((__packed__));
typedef struct BusRequest {
  unsigned char API;
  unsigned char instance;
  short class; // == 0
} BusRequest;
typedef struct BusReply {
  int result;
} BusReply;


typedef struct BusLockRequest {
  unsigned char API; // == 2
  unsigned char instance;
  short class; // == 0
  int id;
  int flags;
} BusLockRequest;
typedef struct BusLockReply {
  int result;
} BusLockReply;

typedef struct BusUnlockRequest {
  unsigned char API; // == 3
  unsigned char instance;
  short class; // == 0
} BusUnlockRequest;
typedef struct BusUnlockReply {
  int dummy;
} BusUnlockReply;

typedef struct BusPreemptRequest {
  unsigned char API; // == 4
  unsigned char instance;
  short class; // == 0
} BusPreemptRequest;
typedef struct BusPreemptReply {
  int dummy;
} BusPreemptReply;

typedef struct BusPeek8Request {
  unsigned char API; // == 5
  unsigned char instance;
  short class; // == 0
  int adrs;
} BusPeek8Request;
typedef struct BusPeek8Reply {
  unsigned char result;
} BusPeek8Reply;

typedef struct BusPoke8Request {
  unsigned char API; // == 6
  unsigned char instance;
  short class; // == 0
  int adrs;
  unsigned char value;
} BusPoke8Request;
typedef struct BusPoke8Reply {
  int dummy;
} BusPoke8Reply;

typedef struct BusPeek16Request {
  unsigned char API; // == 7
  unsigned char instance;
  short class; // == 0
  int adrs;
} BusPeek16Request;
typedef struct BusPeek16Reply {
  unsigned short result;
} BusPeek16Reply;

typedef struct BusPoke16Request {
  unsigned char API; // == 8
  unsigned char instance;
  short class; // == 0
  int adrs;
  unsigned short value;
} BusPoke16Request;
typedef struct BusPoke16Reply {
  int dummy;
} BusPoke16Reply;

typedef struct BusPeek32Request {
  unsigned char API; // == 9
  unsigned char instance;
  short class; // == 0
  int adrs;
} BusPeek32Request;
typedef struct BusPeek32Reply {
  unsigned result;
} BusPeek32Reply;

typedef struct BusPoke32Request {
  unsigned char API; // == 10
  unsigned char instance;
  short class; // == 0
  int adrs;
  unsigned value;
} BusPoke32Request;
typedef struct BusPoke32Reply {
  int dummy;
} BusPoke32Reply;

typedef struct BusBitGet8Request {
  unsigned char API; // == 11
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitGet8Request;
typedef struct BusBitGet8Reply {
  int result;
} BusBitGet8Reply;

typedef struct BusBitAssign8Request {
  unsigned char API; // == 12
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
  int val;
} BusBitAssign8Request;
typedef struct BusBitAssign8Reply {
  int dummy;
} BusBitAssign8Reply;

typedef struct BusBitSet8Request {
  unsigned char API; // == 13
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitSet8Request;
typedef struct BusBitSet8Reply {
  int dummy;
} BusBitSet8Reply;

typedef struct BusBitClear8Request {
  unsigned char API; // == 14
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitClear8Request;
typedef struct BusBitClear8Reply {
  int dummy;
} BusBitClear8Reply;

typedef struct BusBitGet16Request {
  unsigned char API; // == 15
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitGet16Request;
typedef struct BusBitGet16Reply {
  int result;
} BusBitGet16Reply;

typedef struct BusBitAssign16Request {
  unsigned char API; // == 16
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
  int val;
} BusBitAssign16Request;
typedef struct BusBitAssign16Reply {
  int dummy;
} BusBitAssign16Reply;

typedef struct BusBitSet16Request {
  unsigned char API; // == 17
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitSet16Request;
typedef struct BusBitSet16Reply {
  int dummy;
} BusBitSet16Reply;

typedef struct BusBitClear16Request {
  unsigned char API; // == 18
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitClear16Request;
typedef struct BusBitClear16Reply {
  int dummy;
} BusBitClear16Reply;

typedef struct BusBitGet32Request {
  unsigned char API; // == 19
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitGet32Request;
typedef struct BusBitGet32Reply {
  int result;
} BusBitGet32Reply;

typedef struct BusBitAssign32Request {
  unsigned char API; // == 20
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
  int val;
} BusBitAssign32Request;
typedef struct BusBitAssign32Reply {
  int dummy;
} BusBitAssign32Reply;

typedef struct BusBitSet32Request {
  unsigned char API; // == 21
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitSet32Request;
typedef struct BusBitSet32Reply {
  int dummy;
} BusBitSet32Reply;

typedef struct BusBitClear32Request {
  unsigned char API; // == 22
  unsigned char instance;
  short class; // == 0
  int adrs;
  int bit;
} BusBitClear32Request;
typedef struct BusBitClear32Reply {
  int dummy;
} BusBitClear32Reply;

typedef struct BusPeekStreamRequest {
  unsigned char API; // == 23
  unsigned char instance;
  short class; // == 0
  int adrs;
  int dir;
  int len;
} BusPeekStreamRequest;
typedef struct BusPeekStreamReply { // VARLEN
  char data[256];
} BusPeekStreamReply;

typedef struct BusPokeStreamRequest { // VARLEN
  unsigned char API; // == 24
  unsigned char instance;
  short class; // == 0
  int adrs;
  int dir;
  int len;
  char data[256];
} BusPokeStreamRequest;
typedef struct BusPokeStreamReply {
  int status;
} BusPokeStreamReply;

typedef struct BusRefreshRequest {
  unsigned char API; // == 25
  unsigned char instance;
  short class; // == 0
} BusRefreshRequest;
typedef struct BusRefreshReply {
  int dummy;
} BusRefreshReply;

typedef struct BusCommitRequest {
  unsigned char API; // == 26
  unsigned char instance;
  short class; // == 0
  int forceall;
} BusCommitRequest;
typedef struct BusCommitReply {
  int dummy;
} BusCommitReply;

#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
