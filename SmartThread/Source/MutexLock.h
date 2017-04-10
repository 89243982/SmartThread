#ifndef SMARTTHREAD_MUTEXLOCK_H
#define SMARTTHREAD_MUTEXLOCK_H
#include "bMutex.h"
#include "noncopyable.h"
namespace smartthread
{
	class MutexLock:noncopyable
	{
		public:
			MutexLock()
			{
				bmutexlock_init(&this->lock_);
			}
			void lock()
			{
				bmutexlock_lock(&this->lock_);
			}
			void unLock()
			{
				bmutexlock_unlock(&this->lock_);
			}
			
			bMutexLock *get()
			{
				return &this->lock_;
			}
		private:
			bMutexLock lock_;		
	};

	
	class MutexLockGuard:noncopyable
	{
		public:
			MutexLockGuard(MutexLock &lock):lock_(lock)
			{
				this->lock_.lock();
			}
			
			~MutexLockGuard()
			{
				this->lock_.unLock();
			}
		private:
			MutexLock &lock_;
			
	};

}
#endif