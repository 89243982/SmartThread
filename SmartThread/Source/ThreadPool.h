#ifndef SMARTTHREAD_THREADPOOL_H
#define SMARTTHREAD_THREADPOOL_H
#include <string.h>
#include "bThreadPool.h"
#include "noncopyable.h"
namespace smartthread
{
	class ThreadPool:noncopyable
	{
		public:
			ThreadPool()
			{
				//void bthreadpool_init(bThreadPool *pool);
				this->pool_=(bThreadPool*)bmemblock_get_mem(&memTPool);
				memset(this->pool_,0,sizeof(bThreadPool));
				bthreadpool_init(this->pool_);
			}
			bThreadPool *getPool()
			{
				return this->pool_;
			}
			private:
				bThreadPool *pool_;
	};



}
#endif