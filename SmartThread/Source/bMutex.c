#include "bMutex.h"
void bmutexlock_init(bMutexLock *ml)
{
	bevent_init(&ml->waitEvent_,MUTEX);
	ml->count_=0;
	ml->holder_=0;
}


uint32_t bmutexlock_lock(bMutexLock *ml)
{
	uint32_t status=bthread_enter_critical();
	if(ml->count_<=0)
	{
		ml->holder_=currentThread;
		ml->count_++;
		bthread_exit_critical(status);
		return NOERROR;
	}
	else
	{
		if(ml->holder_==currentThread)
		{
			ml->count_++;
			bthread_exit_critical(status);
			return NOERROR;
		}
		else
		{
			if(currentThread->prio_<ml->holder_->prio_)
			{
				if(ml->holder_->state_&READYED)
				{
					bthread_unready(ml->holder_);
					ml->holder_->prio_=currentThread->prio_;
					bthread_ready(ml->holder_);
				}
				else
				{
					ml->holder_->prio_=currentThread->prio_;
				}
			}
			bevent_wait(&ml->waitEvent_,0);
			bthread_exit_critical(status);
			bthread_schedule();
			return currentThread->waitResult_;	
		}
	}
}

uint32_t bmutexlock_unlock(bMutexLock *ml)
{
	uint32_t status=bthread_enter_critical();
	if(ml->holder_!=currentThread||ml->count_<=0)
	{
		bthread_exit_critical(status);
		return IllEGALOP;
	}
	if(--ml->count_<=0)
	{
		if(currentThread->prio_!=currentThread->originalPrio_)
		{
			bthread_unready(currentThread);
			currentThread->prio_=currentThread->originalPrio_;
			bthread_ready(currentThread);
		}
		if(bevent_wait_count(&ml->waitEvent_)>0)
		{
			bThread *thread=bevent_wakeup(&ml->waitEvent_,0,NOERROR);
			ml->holder_=thread;
			ml->count_++;
			if(thread->prio_<currentThread->prio_)
			{
				bthread_schedule();
			}
		}
		
	}
	bthread_exit_critical(status);
	return NOERROR;
}

void bmutexlock_release(bMutexLock *lock)
{
	uint32_t status=bthread_enter_critical();
	lock->count_=0;
	lock->holder_=0;
	bthread_exit_critical(status);
}

uint32_t bmutexlock_wait_count(bMutexLock *lock)
{
	uint32_t count;
	uint32_t status=bthread_enter_critical();
	count=bevent_wait_count(&lock->waitEvent_);
	bthread_exit_critical(status);
	return count;
}

bThread * bmutexlock_wakeup(bMutexLock *lock,void *msg,uint32_t result)
{
	bThread *thread;
	uint32_t status=bthread_enter_critical();
	thread=bevent_wakeup(&lock->waitEvent_,msg,result);
	bthread_exit_critical(status);
	return thread;
}

uint32_t bmutexlock_lock_this(bMutexLock *lock,bThread *thread)
{
	uint32_t status=bthread_enter_critical();
	lock->count_++;
	lock->holder_=thread;
	bthread_exit_critical(status);
	return NOERROR;
}

void bmutexlock_wait(bMutexLock *lock)
{
	uint32_t status=bthread_enter_critical();
	bevent_wait(&lock->waitEvent_,0);
	bthread_exit_critical(status);
}