#ifndef __DIORaw_H
#define __DIORaw_H
#include "Lock.h"

typedef struct DIORaw DIORaw;
typedef struct DIORawAPI DIORawAPI;

struct DIORawAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  int (*Lock)(void *,int command,unsigned num);
  void (*DirSet)(void *,int num,int asOutput);
  void (*DataSet)(void *,int num,int asHigh);
  int (*DirGet)(void *,int num);
  int (*DataGet)(void *,int num);
  unsigned (*Count)(void *);
};

struct DIORaw {
  DIORawAPI *API;
  int InitStatus;
  unsigned LockBase;
};

enum {
  NetDIORaw_Lock=0,
  NetDIORaw_DirSet=1,
  NetDIORaw_DataSet=2,
  NetDIORaw_DirGet=3,
  NetDIORaw_DataGet=4,
  NetDIORaw_Count=5
};
#define DIORaw_APICount (1+NetDIORaw_Count)
#endif

// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
