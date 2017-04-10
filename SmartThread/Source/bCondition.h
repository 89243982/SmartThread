#ifndef SMARTTHREAD_BCONDDITION_H
#define SMARTTHREAD_BCONDDITION_H
#include <stdint.h>
#include "bMutex.h"
#include "bThread.h"

typedef struct _bCondition
{
	bMutexLock *lock_;
}bCondition;

void bcondition_init(bCondition *cond,bMutexLock *lock);

uint32_t bcondition_wait(bCondition *cond);

uint32_t bcondition_notify(bCondition *cond);

#endif