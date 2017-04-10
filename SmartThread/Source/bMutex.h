#ifndef SMARTTHREAD_BMUTEXLOCK_H
#define SMARTTHREAD_BMUTEXLOCK_H
#include <stdint.h>
#include "bEvent.h"

typedef struct _bMutexLock
{
	bEvent waitEvent_;
	uint32_t count_;
	bThread *holder_;
}bMutexLock;


void bmutexlock_init(bMutexLock *lock);

uint32_t bmutexlock_lock(bMutexLock *lock);

uint32_t bmutexlock_unlock(bMutexLock *lock);

void bmutexlock_release(bMutexLock *lock);

uint32_t bmutexlock_wait_count(bMutexLock *lock);

bThread * bmutexlock_wakeup(bMutexLock *lock,void *msg,uint32_t result);

uint32_t bmutexlock_lock_this(bMutexLock *lock,bThread *thread);

void bmutexlock_wait(bMutexLock *lock);

#endif