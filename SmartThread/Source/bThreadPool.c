#include <string.h>
#include "bThreadPool.h"
#include "bConfig.h"
void btask_init(bTask *task,func handler,void *arg,bThreadPool *pool)
{
	bnode_init(&task->linkedNode_);
	task->handler_=handler;
	task->arg_=arg;
	bthreadpool_put_task(pool,task);
}
void bthreadpool_init(bThreadPool *pool)
{
	int i;
	bThread *thread;
	stack *bstack;
	blist_init(&pool->tasks_);
	bmutexlock_init(&pool->lock_);
	bcondition_init(&pool->noEmpty_,&pool->lock_);

	for(i=0;i<THREAD_COUNT_IN_THREADPOOL;++i)
	{
		thread=(bThread*)bmemblock_get_mem(&memThreadsPool);
		bstack=(stack*)bmemblock_get_mem(&memStacksPool);
		memset(thread,0,sizeof(bThread));
		memset(bstack,0,sizeof(stack)*STACK_SIZE);
	  bthread_create(thread,bthreadpool_run,pool,THREAD_PRIO_IN_THREADPOOL,bstack);
	}
}

void bthreadpool_put_task(bThreadPool *pool,bTask *task)
{
	bmutexlock_lock(&pool->lock_);
	blist_add_last(&pool->tasks_,&task->linkedNode_);
	bcondition_notify(&pool->noEmpty_);
	bmutexlock_unlock(&pool->lock_);
}


bTask* bthreadpool_take_task(bThreadPool *pool)
{
	bTask *task=0;
	bNode *node=0;
	bmutexlock_lock(&pool->lock_);
	while(blist_count(&pool->tasks_)<=0)
	{
		bcondition_wait(&pool->noEmpty_);
	}
	node=blist_remove_first(&pool->tasks_);	
	task=bnode_parent(node,bTask,linkedNode_);
	bmutexlock_unlock(&pool->lock_);
	return task;
}

void* bthreadpool_run(void *pool)
{
	for(;;)
	{
			bTask *task=bthreadpool_take_task((bThreadPool*)pool);
			task->handler_(task->arg_);
	}
	return 0;

}
