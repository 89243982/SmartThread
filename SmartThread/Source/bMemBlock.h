#ifndef SMARTTHREAD_BMEMBLOCK_H
#define SMARTTHREAD_BMEMBLOCK_H
#include <stdint.h>
#include "bList.h"
#include "bCondition.h"
#include "bThread.h"
typedef struct _bMemBlock
{
	uint8_t *startAddr_;
	uint32_t blockSize_;
	uint32_t count_;
	bList blocks_;
	bMutexLock lock_;
	bCondition noEmpty_;
}bMemBlock;


void bmemblock_init(bMemBlock *block,uint8_t *startAddr,uint32_t blockSize,uint32_t count);
void* bmemblock_get_mem(bMemBlock *block);
void bmemblock_release_mem(bMemBlock *block,void *mem);
void bmemblock_all_init();
	

extern bMemBlock memThreadsPool;
extern bMemBlock memStacksPool;
extern bMemBlock memThreadsUser;
extern bMemBlock memStacksUser;
extern bMemBlock memTasksUser;
extern bMemBlock memTPool;
extern bMemBlock memTimerUser;
#endif