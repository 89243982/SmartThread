#include "bCondition.h"

void bcondition_init(bCondition *cond,bMutexLock *lock)
{
	cond->lock_=lock;
}

uint32_t bcondition_wait(bCondition *cond)
{
	uint32_t status=bthread_enter_critical();
	bmutexlock_release(cond->lock_);
	if(bmutexlock_wait_count(cond->lock_))
	{
		bThread *thread=bmutexlock_wakeup(cond->lock_,0,NOTAVAILABLESOURCE);
		bmutexlock_lock_this(cond->lock_,thread);
	}
	bmutexlock_wait(cond->lock_);
	bthread_exit_critical(status);
	bthread_schedule();
	return currentThread->waitResult_;
}


uint32_t bcondition_notify(bCondition *cond)
{
	uint32_t status=bthread_enter_critical();
	if(bmutexlock_wait_count(cond->lock_))
	{
		bThread *thread;
		bmutexlock_release(cond->lock_);
		thread=bmutexlock_wakeup(cond->lock_,0,NOERROR);
		bmutexlock_lock_this(cond->lock_,thread);
		bmutexlock_wait(cond->lock_);
		bthread_exit_critical(status);
		bthread_schedule();
		return currentThread->waitResult_;
	}
	
	bthread_exit_critical(status);
	return NOERROR;
}