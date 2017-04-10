#ifndef SMARTTHREAD_BTHREAD_H
#define SMARTTHREAD_BTHREAD_H
#include <stdio.h>
#include <stdint.h>
#include "bTypes.h"
#include "bConfig.h"
#include "bList.h"
#define CREATED 0
#define READYED (1<<0)
#define DELAYED (1<<1)
#define RUNNING (1<<2)
#define WAITEED (1<<3)
struct _bEvent;
typedef struct _bThread
{
	stack *stackTop_;
	bNode linkedNode_;
	bNode delayNode_;
	func handler_;
	void *arg_;
	uint32_t prio_;
	uint32_t state_;
	uint32_t slice_;
	uint32_t delayTicks_;
	struct _bEvent *waitEvent_;
	void *msg_;
	uint32_t waitResult_;
	uint32_t originalPrio_;
	uint32_t flags_;
	stack *stackBaseAddr_;
}bThread;

typedef struct _bBitmap
{
	uint32_t bitmap_;
}bBitmap;


bThread* bthread_create(bThread *thread,func handler,void *arg,uint32_t prio,stack *correspondingStack);
void bthread_ready(bThread *thread);
void bthread_unready(bThread *thread);
bThread* bthread_get_readyed();
void bthread_ready(bThread *thread);
void bthread_unready(bThread *thread);
void bthread_time_wait(uint32_t ms);
void bthread_time_wakeup(bThread *thread);
void bthreads_start();
void bthread_delay(uint32_t ms);
void SysTick_Handler ();
void bthread_schedule();
void bthread_set_flags(bThread *thread,uint32_t flag);
uint32_t bthread_get_ticks(uint32_t ms);
uint32_t bthread_stack_used();
void bbitmap_init (bBitmap * bitmap);

extern bThread *currentThread;
extern bThread *nextThread;
extern bBitmap threadsBMapRdyed;



#endif