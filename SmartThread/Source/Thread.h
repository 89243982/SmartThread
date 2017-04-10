#ifndef SMARTTHREAD_THREAD_H
#define SMARTTHREAD_THREAD_H
#include <string.h>
#include "bThread.h"
#include "bTypes.h"
#include "bMemBlock.h"
#include "noncopyable.h"

namespace smartthread
{
	class Thread:noncopyable
	{
		public:
			Thread(func handler,void *arg,uint32_t prio=1)
			{
					this->thread_=(bThread*)bmemblock_get_mem(&memThreadsUser);
					stack *bStack=(stack*)bmemblock_get_mem(&memStacksUser);
					bthread_create(this->thread_,handler,arg,prio,bStack);
			}		
		private:
			bThread *thread_;
	};
}
#endif