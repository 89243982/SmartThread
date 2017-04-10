#ifndef SMARTTHREAD_TASK_H
#define SMARTTHREAD_TASK_H
#include <string.h>
#include "bTypes.h"
#include "bThreadPool.h"
#include "bMemBlock.h"
#include "ThreadPool.h"
#include "noncopyable.h"
namespace smartthread
{
	
	class Task:noncopyable
	{
		public:
			//void btask_init(bTask *task,func handler,void *arg,bThreadPool *pool)
			Task(func handler,void *arg,ThreadPool *pool)
			{
				this->task_=(bTask*)bmemblock_get_mem(&memTasksUser);
				memset(this->task_,0,sizeof(bTask));
				btask_init(this->task_,handler,arg,pool->getPool());
			}
		private:
			bTask *task_;
	};

}
#endif