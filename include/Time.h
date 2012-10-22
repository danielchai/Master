#ifndef __Time_H
#define __Time_H

typedef unsigned long long TimeNS; // nanoseconds

typedef struct Time Time;
typedef struct TimeAPI TimeAPI;

struct TimeAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  unsigned (*Wait)(void *,unsigned microseconds);
  void (*Delay)(void *,unsigned microseconds);
  unsigned (*Tick)(void *);
  unsigned (*usElapsed)(void *,unsigned start);
  unsigned (*usFuture)(void *,unsigned start,unsigned us);
  int (*TimeoutQ)(void *,unsigned start,unsigned end);
  unsigned (*TPS)(void *);
};

struct Time {
  TimeAPI *API;
  int InitStatus;
};

enum {
  NetTime_Wait=0,
  NetTime_Delay=1,
  NetTime_Tick=2,
  NetTime_usElapsed=3,
  NetTime_usFuture=4,
  NetTime_TimeoutQ=5,
  NetTime_TPS=6
};
#define Time_APICount (1+NetTime_TPS)
#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
