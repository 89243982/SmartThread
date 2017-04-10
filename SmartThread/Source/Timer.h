#ifndef SMARTTHREAD_TIMER_H
#define SMARTTHREAD_TIMER_H
#include <string.h>
#include "bTimer.h"
#include "bMemBlock.h"
#include "noncopyable.h"
namespace smartthread
{
	class Timer:noncopyable
	{
		public:
			Timer(uint32_t intervalMs,uint32_t periodMs,func handler,void *arg,bTimerType type)
			{
				//void btimer_init(bTimer *timer,uint32_t intervalMs,uint32_t periodMs,func handler,void *arg,bTimerType type);
				this->timer_=(bTimer*)bmemblock_get_mem(&memTimerUser);
				memset(this->timer_,0,sizeof(bTimer));
				btimer_init(this->timer_,intervalMs,periodMs,handler,arg,type);
			}
		private:
			bTimer *timer_;
	};


}
#endif