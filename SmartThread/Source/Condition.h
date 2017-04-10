#ifndef SMARTTHREAD_CONDITION_H
#define SMARTTHREAD_CONDITION_H
#include "MutexLock.h"
#include "bCondition.h"
#include "noncopyable.h"
namespace smartthread
{
	class Condition:noncopyable
	{
		public:
			Condition(MutexLock &lock):lock_(lock)
			{
				bcondition_init(&this->cond_,this->lock_.get());
			}
			uint32_t wait()
			{
				return bcondition_wait(&this->cond_);
			}
			uint32_t notify()
			{
				return bcondition_notify(&this->cond_);
			}
			
		private:
			MutexLock &lock_;
			bCondition cond_;
			
	};
}
#endif