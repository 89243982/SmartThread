#include <stdbool.h>
#include "bTimer.h"
#include "bMemBlock.h"
void btimer_init(bTimer *timer,uint32_t intervalMs,uint32_t periodMs,func handler,void *arg,bTimerType type)
{
	bnode_init(&timer->linkNode_);
	timer->intervalTicks_=bthread_get_ticks(intervalMs);
	timer->periodTicks_=(periodMs?bthread_get_ticks(periodMs):0);
	timer->delayTicks_=timer->intervalTicks_;
	timer->handler_=handler;
	timer->arg_=arg;
	timer->type_=type;
	if(timer->type_==RT)
	{
		blist_add_last(&timers_rt,&timer->linkNode_);
	}
	else
	{
		blist_add_last(&timers_nrt,&timer->linkNode_);
	}
}

void btimer_run_rt()
{
		bNode *nextNode;
		for(nextNode=timers_rt.firstNode_;nextNode!=&timers_rt.headNode_;)
		{
			bTimer *timer;
			bNode *currentNode=nextNode;
			nextNode=currentNode->nextNode_;
			timer=bnode_parent(currentNode,bTimer,linkNode_);
			if(--timer->delayTicks_<=0)
			{
				timer->handler_(timer->arg_);
				if(timer->periodTicks_)
				{
					timer->delayTicks_=timer->periodTicks_;
				}
				else
				{
					blist_remove(&timers_rt,currentNode);
					bmemblock_release_mem(&memTimerUser,(bNode*)timer);
				}
			}
			
		}
}



void btimer_run_nrt_in_systick()
{
	bool flag=false;
	bNode *nextNode;
	for(nextNode=timers_nrt.firstNode_;nextNode!=&timers_nrt.headNode_;)
	{
			bTimer *timer;
			bNode *currentNode=nextNode;
			nextNode=currentNode->nextNode_;
			timer=bnode_parent(currentNode,bTimer,linkNode_);
			
			if(timer->delayTicks_&&(--timer->delayTicks_<=0))
			{
					flag=true;
			}
	}
	if(flag)
	{
			bsem_notify(&semTick);
	}
}
void btimer_run_nrt_in_thread()
{
	bNode *nextNode;
	uint32_t status;
	bsem_wait(&semTick,0);
	status=bthread_enter_critical();
	for(nextNode=timers_nrt.firstNode_;nextNode!=&timers_nrt.headNode_;)
	{
			bTimer *timer;
			bNode *currentNode=nextNode;
			nextNode=currentNode->nextNode_;
			timer=bnode_parent(currentNode,bTimer,linkNode_);
			if(timer->delayTicks_<=0)
			{
				timer->handler_(timer->arg_);
				if(timer->periodTicks_)
				{
					timer->delayTicks_=timer->periodTicks_;
				}
				else
				{
					blist_remove(&timers_nrt,currentNode);
					bmemblock_release_mem(&memTimerUser,(bNode*)timer);
				}
			}
	}
	bthread_exit_critical(status);
	
}
void btimer_in_systick()
{
	btimer_run_rt();
	btimer_run_nrt_in_systick();
}