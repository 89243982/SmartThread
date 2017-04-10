#ifndef SMARTTHREAD_BSEM_H
#define SMARTTHREAD_BSEM_H

#include "bEvent.h"
#include "bThread.h"

typedef struct _bSem
{
	bEvent waitEvent_;
	uint32_t count_;
}bSem;

void bsem_init(bSem *sem);
uint32_t bsem_wait(bSem *sem,uint32_t ms);
void bsem_notify(bSem *sem);

void bsem_all_init();

extern bSem semTick;

#endif