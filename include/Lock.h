#ifndef __Lock_H
#define __Lock_H

typedef struct Lock Lock;
typedef struct LockAPI LockAPI;

enum {
  TryLock=0,WaitLock=1,DoRelease=2,DoPreempt=5
};


typedef int (*LockFunction)(void *,int,unsigned);

struct LockAPI {
  void *(*Init)(void *,...);
  void (*Fini)(void *);
  int (*Lock)(void *,int command,unsigned num);
};

/*
  Lock commands:
  1. TryLock
       non-blocking lock
       returns failure if the lock would block
  2. DoRelease 
       unconditional unlock
       unlocks the given lock number. 
  3. WaitLock 
       blocking lock
       locks the given lock number and returns success when this has
       been done.  returns failure if the call would deadlock
  4. DoPreempt
       releases all locks, allows the current thread to be re-scheduled,
       and the re-aquires all locks before returning.

  The Lock object must be implemented as a singleton.  The reason for this is
  due to the way locking works.
 */
struct Lock {
  LockAPI *API;
  int InitStatus;
  unsigned LockBase,NumLocks;
  int deferlock;
};
extern Lock *lock;

int LockDirect(Lock *obj,int command,unsigned num,void *ob);


#endif
// Author: Michael Schmidt (michael@embeddedARM.com)
// Copyright (c) 2011, Technologic Systems, All Rights Reserved
// Refer to the COPYRIGHT file provided with this project for licensing terms.
