#ifndef SMARTTHREAD_BTIMER_H
#define SMARTTHREAD_BTIMER_H
#include "bList.h"
#include "bTypes.h"
#include "bThread.h"
#include "bSem.h"




typedef enum 
{
	RT,
	NRT
}bTimerType;
typedef struct _bTimer
{
	bNode linkNode_;
	uint32_t intervalTicks_;
	uint32_t periodTicks_;
	uint32_t delayTicks_;
	func handler_;
	void *arg_;
	bTimerType type_;
}bTimer;


void btimer_init(bTimer *timer,uint32_t intervalMs,uint32_t periodMs,func handler,void *arg,bTimerType type);
void btimer_run_nrt_in_thread();
void btimer_in_systick();
#endif