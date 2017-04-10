#ifndef SMARTTHREAD_BEVENT_H
#define SMARTTHREAD_BEVENT_H
#include <stdint.h>
#include "bList.h"
#include "switch.h"
#include "bThread.h"

typedef enum
{
	NOERROR=1,
	TIMEOUT,
	NOTAVAILABLESOURCE,
	IllEGALOP,
	UNLOCKED 
}bError;
typedef enum{
	UNKNOWED,
	MUTEX,
	SEM,
	COND,
	FLAGGROUP
}bEventType;
typedef struct _bEvent
{
	bEventType type_;
	bList waitLists_;
}bEvent;

void bevent_init(bEvent *event,bEventType type);

void bevent_wait(bEvent *event,uint32_t ms);

bThread* bevent_wakeup(bEvent *event,void *msg,uint32_t result);

void bevent_remove(bEvent *event,bThread *thread,void *msg,uint32_t result);

void bevent_remove_all(bEvent *event,func fun);

uint32_t bevent_wait_count(bEvent *event);


#endif