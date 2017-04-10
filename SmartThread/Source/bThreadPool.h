#ifndef SMARTTHREAD_BTHREADPOOL_H
#define SMARTTHREAD_BTHREADPOOL_H

#include <stdint.h>
#include "bList.h"
#include "bTypes.h"
#include "bCondition.h"
#include "bMemBlock.h"
#include "bThread.h"
typedef struct _bTask
{
	bNode linkedNode_;
	func handler_;
	void *arg_;

}bTask;


typedef struct _bThreadPool
{
	bList tasks_;
	bMutexLock lock_;
	bCondition noEmpty_;
	
}bThreadPool;

void btask_init(bTask *task,func handler,void *arg,bThreadPool *pool);
void bthreadpool_init(bThreadPool *pool);
void bthreadpool_put_task(bThreadPool *pool,bTask *task);


bTask* bthreadpool_take_task(bThreadPool *pool);
void* bthreadpool_run(void *pool);


#endif