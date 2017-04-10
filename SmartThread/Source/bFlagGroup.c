#include "bFlagGroup.h"
void bflaggroup_init(bFlagGroup *fg)
{
	fg->flags_=0;
	bevent_init(&fg->waitEvent_,FLAGGROUP);
}

static bool bflaggroup_check_consume(bFlagGroup *fg,CheckMethod method)
{
				if(method==ALL_SET&&((currentThread->flags_&fg->flags_)==currentThread->flags_))
				{
					fg->flags_&=~currentThread->flags_;
					return true;
				}
				else if(method==ANY_SET&&((currentThread->flags_&fg->flags_)>0))
				{
					fg->flags_&=~currentThread->flags_;
					return true;
				}
				else if(method==ALL_CLEAR&&((currentThread->flags_&fg->flags_)==0))
				{
					return true;
				}
				else if(method==ANY_CLEAR&&((currentThread->flags_&fg->flags_)!=currentThread->flags_))
				{
					return true;
				}
				else
				{
					return false;
				}
}
void bflaggroup_take_flags(bFlagGroup *fg,CheckMethod method)
{
		uint32_t status=0;
		while(true)
		{
			status=bthread_enter_critical();
			if(bflaggroup_check_consume(fg,method)==true)
			{
				bthread_exit_critical(status);
				break;
			}
			bevent_wait(&fg->waitEvent_,0);
			bthread_exit_critical(status);
			bthread_schedule();
		}
}

void bflaggroup_set_flags(bFlagGroup *fg,uint32_t flags)
{
	uint32_t status=bthread_enter_critical();
	fg->flags_|=flags;
	if(bevent_wait_count(&fg->waitEvent_))
	{
		bThread* thread=bevent_wakeup(&fg->waitEvent_,(void*)0,NOERROR);
		if(thread->prio_<currentThread->prio_)
		{
			bthread_schedule();
		}
	}
	bthread_exit_critical(status);
}

bFlagGroup test;
