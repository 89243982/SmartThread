#include "bEvent.h"



void bevent_init(bEvent *event,bEventType type)
{
	event->type_=type;
	blist_init(&event->waitLists_);
}

void bevent_wait(bEvent *event,uint32_t ms)
{
	uint32_t status=bthread_enter_critical();
	bthread_unready(currentThread);
	blist_add_last(&event->waitLists_,&currentThread->linkedNode_);
	if(ms)
	{
		bthread_time_wait(ms);
	}
	currentThread->waitEvent_=event;
	currentThread->state_|=WAITEED;
	bthread_exit_critical(status);
}

bThread* bevent_wakeup(bEvent *event,void *msg,uint32_t result)
{
	bThread *thread=0;
	bNode *node=0;
	uint32_t status=bthread_enter_critical();
	node=blist_remove_first(&event->waitLists_);
	if(node)
	{
		//blist_remove(&event->waitLists_,&currentThread->linkedNode_);
		thread=bnode_parent(node,bThread,linkedNode_);
		if(thread->state_&DELAYED)
		{
			bthread_time_wakeup(thread);
		}
		bthread_ready(thread);
		thread->waitEvent_=0;
		thread->msg_=msg;
		thread->waitResult_=result;
		thread->state_&=~WAITEED;
	}
	bthread_exit_critical(status);
	return thread;
}


void bevent_remove(bEvent *event,bThread *thread,void *msg,uint32_t result)
{
	uint32_t status=bthread_enter_critical();
	blist_remove(&event->waitLists_,&thread->linkedNode_);
	thread->waitEvent_=0;
	thread->msg_=msg;
	thread->waitResult_=result;
	thread->state_&=~WAITEED;
	bthread_exit_critical(status);
}

void bevent_remove_all(bEvent *event,func fun)
{
	blist_remove_all(&event->waitLists_,fun);
}

uint32_t bevent_wait_count(bEvent *event)
{
		uint32_t count;
		uint32_t status=bthread_enter_critical();
		count=blist_count(&event->waitLists_);
		bthread_exit_critical(status);
		return count;
}
