#include <stdlib.h>
#include "bMemBlock.h"
#include "bThreadPool.h"
#include "bTimer.h"




void bmemblock_init(bMemBlock *block,uint8_t *startAddr,uint32_t blockSize,uint32_t count)
{
	
	uint8_t *endAddr;
	
	if(blockSize<sizeof(bNode))
	{
		exit(1);
	}
	block->startAddr_=startAddr;
	block->blockSize_=blockSize;
	block->count_=count;
	blist_init(&block->blocks_);
	bmutexlock_init(&block->lock_);
	bcondition_init(&block->noEmpty_,&block->lock_);
	
	
	endAddr=block->startAddr_+blockSize*count;
	while(block->startAddr_<endAddr)
	{
		bnode_init((bNode*)block->startAddr_);
		blist_add_last(&block->blocks_,(bNode*)block->startAddr_);
		block->startAddr_+=blockSize;
	}
	
}

void* bmemblock_get_mem(bMemBlock *block)
{
	bNode *node=0;
	bmutexlock_lock(&block->lock_);
	while(block->count_<=0)
	{
		bcondition_wait(&block->noEmpty_);
	}
	block->count_--;
	node=blist_remove_first(&block->blocks_);
	bmutexlock_unlock(&block->lock_);
	return node;
}
	

void bmemblock_release_mem(bMemBlock *block,void *mem)
{
	bmutexlock_lock(&block->lock_);
	block->count_++;
	bnode_init((bNode*)mem);
	blist_add_last(&block->blocks_,(bNode*)mem);
	bcondition_notify(&block->noEmpty_);
	bmutexlock_unlock(&block->lock_);
}
	

uint8_t memThreadsThreadPool[THREAD_COUNT_IN_THREADPOOL][sizeof(bThread)];
bMemBlock memThreadsPool;
uint8_t memStacksThreadPool[THREAD_COUNT_IN_THREADPOOL][STACK_SIZE*sizeof(stack)];
bMemBlock memStacksPool;

uint8_t memThreadsDefinedByUser[THREAD_DEFINE_BY_USER][sizeof(bThread)];
bMemBlock memThreadsUser;
uint8_t memStacksDefinedByUser[THREAD_DEFINE_BY_USER][STACK_SIZE*sizeof(stack)];
bMemBlock memStacksUser;


uint8_t memThreadPool[1][sizeof(bThreadPool)];
bMemBlock memTPool;

uint8_t memTasksDefinedByUser[TASK_NUM][sizeof(bTask)];
bMemBlock memTasksUser;


uint8_t memTimerDefineByUser[TIMER_NUM][sizeof(bTimer)];
bMemBlock memTimerUser;

void bmemblock_all_init()
{
	//bMemBlock *block,uint8_t *startAddr,uint32_t blockSize,uint32_t count
	bmemblock_init(&memThreadsPool,(uint8_t*)memThreadsThreadPool,sizeof(bThread),THREAD_COUNT_IN_THREADPOOL);
	bmemblock_init(&memStacksPool,(uint8_t*)memStacksThreadPool,STACK_SIZE*sizeof(stack),THREAD_COUNT_IN_THREADPOOL);
	
	bmemblock_init(&memThreadsUser,(uint8_t*)memThreadsDefinedByUser,sizeof(bThread),THREAD_DEFINE_BY_USER);
	bmemblock_init(&memStacksUser,(uint8_t*)memStacksDefinedByUser,STACK_SIZE*sizeof(stack),THREAD_DEFINE_BY_USER);


	bmemblock_init(&memTPool,(uint8_t*)memThreadPool,sizeof(bThreadPool),1);
	
	bmemblock_init(&memTasksUser,(uint8_t*)memTasksDefinedByUser,sizeof(bTask),TASK_NUM);
	
	bmemblock_init(&memTimerUser,(uint8_t*)memTimerDefineByUser,sizeof(bTimer),TIMER_NUM);
	
}