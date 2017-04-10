#include "bSem.h"

void bsem_init(bSem *sem)
{
	bevent_init(&sem->waitEvent_,SEM);
	sem->count_=0;
}

uint32_t bsem_wait(bSem *sem,uint32_t ms)
{
	uint32_t status=bthread_enter_critical();
	if(sem->count_>0)
	{
		--sem->count_;
		bthread_exit_critical(status);
		return NOERROR;
	}
	else
	{
		bevent_wait(&sem->waitEvent_,ms);
		bthread_exit_critical(status);
		bthread_schedule();
		return currentThread->waitResult_;
	}
}

void bsem_notify(bSem *sem)
{
	uint32_t status=bthread_enter_critical();
	if(bevent_wait_count(&sem->waitEvent_))
	{
		bThread* thread=bevent_wakeup(&sem->waitEvent_,(void*)0,NOERROR);
		if(thread->prio_<currentThread->prio_)
		{
			bthread_schedule();
		}
	}
	else
	{
		++sem->count_;
	}
	bthread_exit_critical(status);

}

bSem semTick;

void bsem_all_init()
{
	bsem_init(&semTick);
}
