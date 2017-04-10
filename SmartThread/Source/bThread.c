#include <string.h>
#include "bThread.h"
#include "bConfig.h"
#include "switch.h"
#include "bEvent.h"
#include "bTimer.h"
#include "bFlagGroup.h"






bBitmap threadsBMapRdyed;


void bbitmap_init (bBitmap * bitmap)
{
	bitmap->bitmap_= 0;
}

uint32_t bbitmap_count (void)
{
	return sizeof(uint32_t)*8;
}

void bbitmap_set (bBitmap *bitmap, uint32_t pos)
{
	bitmap->bitmap_ |= (1 << pos);
}

void bbitmap_clear (bBitmap *bitmap, uint32_t pos)
{
	bitmap->bitmap_ &= ~(1 << pos);
}

uint32_t bbitmap_get_firstset (bBitmap * bitmap)
{
	static const uint8_t quickFindTable[] = 
	{
		/* 00 */ 0xff, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 10 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 20 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 30 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 40 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 50 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 60 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 70 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 80 */ 7,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* 90 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* A0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* B0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* C0 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* D0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* E0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	    /* F0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	};
	
	if (bitmap->bitmap_ & 0xFF)
	{
		return quickFindTable[bitmap->bitmap_ & 0xFF];
	}
	else if (bitmap->bitmap_ & 0xFF00)
	{
		return quickFindTable[(bitmap->bitmap_ >> 8) & 0xFF] + 8;
	}
	else if (bitmap->bitmap_ & 0xFF0000)
	{
		return quickFindTable[(bitmap->bitmap_ >> 16) & 0xFF] + 16;
	}
	else if (bitmap->bitmap_ & 0xFF000000)
	{
		return quickFindTable[(bitmap->bitmap_ >> 24) & 0xFF] + 24;
	}
	else
	{
		return quickFindTable[0];
	}
}
static void bthread_stack_init(bThread *thread)
{
	*(--thread->stackTop_) = (stack)(1 << 24);
	*(--thread->stackTop_) = (stack)thread->handler_;
	*(--thread->stackTop_) = (stack)0x14;
	*(--thread->stackTop_) = (stack)0x12;
	*(--thread->stackTop_) = (stack)0x3;
	*(--thread->stackTop_) = (stack)0x2;
	*(--thread->stackTop_)= (stack)0x1;
	*(--thread->stackTop_) = (stack)thread->arg_;
	
	*(--thread->stackTop_) = (stack)0x11;
	*(--thread->stackTop_) = (stack)0x10;
	*(--thread->stackTop_) = (stack)0x9;
	*(--thread->stackTop_)= (stack)0x8;
	*(--thread->stackTop_) = (stack)0x7;
	*(--thread->stackTop_) = (stack)0x6;
	*(--thread->stackTop_) = (stack)0x5;
	*(--thread->stackTop_) = (stack)0x4;
}

bThread* bthread_create(bThread *thread,func handler,void *arg,uint32_t prio,stack *correspondingStack)
{
	memset(thread,0,sizeof(bThread));
	memset(correspondingStack,0,STACK_SIZE*sizeof(stack));
	thread->stackBaseAddr_=correspondingStack;
	thread->stackTop_=&correspondingStack[STACK_SIZE];
	bnode_init(&thread->linkedNode_);
	bnode_init(&thread->delayNode_);
	thread->handler_=handler;
	thread->arg_=arg;
	thread->prio_=prio;
	thread->state_=CREATED;
	thread->slice_=SLICE_NUM;
	thread->delayTicks_=0;
	thread->waitEvent_=0;
	thread->msg_=(void*)0;
	thread->waitResult_=0;
	thread->originalPrio_=prio;
	thread->flags_=0;
	bthread_stack_init(thread);
	bthread_ready(thread);
	
	return thread;
}

void bthread_ready(bThread *thread)
{
	blist_add_last(&threadsRdy[thread->prio_],&thread->linkedNode_);
	thread->state_|=READYED;
	thread->slice_=SLICE_NUM;
	bbitmap_set(&threadsBMapRdyed,thread->prio_);
	
}
void bthread_unready(bThread *thread)
{
	blist_remove(&threadsRdy[thread->prio_],&thread->linkedNode_);
	thread->state_&=~READYED;
	if(blist_count(&threadsRdy[thread->prio_])==0)
	{
		bbitmap_clear(&threadsBMapRdyed,thread->prio_);
	}
}
uint32_t bthread_get_ticks(uint32_t ms)
{
	uint32_t ticks=ms/SYS_TICK_PEROID_MS;
	return ticks?ticks:1;
}
void bthread_time_wait(uint32_t ms)
{
	currentThread->delayTicks_=bthread_get_ticks(ms);
	blist_add_last(&threadsDelayed,&currentThread->delayNode_);
	currentThread->state_|=DELAYED;
}

void bthread_time_wakeup(bThread *thread)
{
	blist_remove(&threadsDelayed,&thread->delayNode_);
	thread->state_&=~DELAYED;
	thread->delayTicks_=0;
}

bThread* bthread_get_readyed()
{
	uint32_t pos=bbitmap_get_firstset(&threadsBMapRdyed);
	return bnode_parent(blist_first(&threadsRdy[pos]),bThread,linkedNode_);
}

uint32_t bthread_stack_used()
{
	int i;
	int countUnUsed=0;
	for(i=0;i<STACK_SIZE;++i)
	{
		if(currentThread->stackBaseAddr_[i]==0)
		{
			countUnUsed++;
		}
		else
		{
			break;
		}
	}
	return STACK_SIZE-countUnUsed;
	
}

bThread *currentThread;
bThread *nextThread;





static void bthreads_delayed_wakeup()
{
	bNode *nextNode;
	uint32_t flag=0;
	for(nextNode=threadsDelayed.firstNode_;nextNode!=&threadsDelayed.headNode_;)
	{
		bNode *currentNode=nextNode;
		bThread *thread;
		nextNode=currentNode->nextNode_;
		thread=bnode_parent(currentNode,bThread,delayNode_);
		if(--thread->delayTicks_==0)
		{
			bthread_time_wakeup(thread);
			if(thread->state_&WAITEED)
			{
				bevent_remove(thread->waitEvent_,thread,0,TIMEOUT);
			}
			bthread_ready(thread);
			if(thread->prio_<currentThread->prio_)
			{
				flag=1;
			}
		}
	}
	if(flag)
	{
		bthread_schedule();
	}
}

void bthread_schedule()
{
	uint32_t status=bthread_enter_critical();
	bThread *thread=bthread_get_readyed();
	thread->slice_=SLICE_NUM;
	if(thread==currentThread)
	{
		bthread_exit_critical(status);
		return;
	}
	nextThread=thread;
	bthread_switch();
	bthread_exit_critical(status);
}

void bthread_delay(uint32_t ms)
{
	uint32_t status=bthread_enter_critical();
	bthread_unready(currentThread);
	bthread_time_wait(ms);
	bthread_exit_critical(status);
	bthread_schedule();
}

void bthreads_start()
{
	bthread_run_first();
}

void bthread_systick_handler()
{
	bthreads_delayed_wakeup();
	btimer_in_systick();
	if(--currentThread->slice_<=0)
	{
		//blist_remove(&threadsRdy[currentThread->prio_],&currentThread->linkedNode_);
		//blist_add_last(&threadsRdy[currentThread->prio_],&currentThread->linkedNode_);
		if(currentThread->state_&READYED)
		{
				bthread_unready(currentThread);
				bthread_ready(currentThread);
		}
		bthread_schedule();
	}
	
}

void SysTick_Handler ()
{
	uint32_t status=bthread_enter_critical();
	bthread_systick_handler();
	bthread_exit_critical(status);
}

void bthread_set_flags(bThread *thread,uint32_t flag)
{
	thread->flags_=flag;
}

