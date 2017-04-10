#ifndef SMARTTHREAD_SMARTTHREAD_H
#define SMARTTHREAD_SMARTTHREAD_H
extern "C"
{
#include "bThread.h"
#include "bThreadPool.h"
#include "bTimer.h"
#include "bSem.h"
#include "bMailBox.h"
#include "bFlagGroup.h"
#include "functions.h"
}

#include "Thread.h"
#include "Task.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"
namespace smartthread
{
	
	class SmartThread:noncopyable
	{
		public:
			SmartThread()
			{
				threads_all_list_init();
				bbitmap_init(&threadsBMapRdyed);
				bmemblock_all_init();
				bsem_all_init();	
				bthread_create(&this->threadFirst_,&setEnv,0,1,threadStackFirst_);
				bthread_create(&this->threadTimer_,&timers_thread,0,1,threadTimerStack_);
			}
			
			static void * setEnv(void *arg)
			{
				//define threadpool
				ThreadPool threads;
				//define tasks
				Task t1(fun1,0,&threads);
				Task t2(fun2,0,&threads);
				Task t3(fun3,0,&threads);
				Task t4(fun4,0,&threads);
				Task t5(fun5,0,&threads);
				//define timer
				Timer ti1(20,20,timer1,0,RT);
				Timer ti2(20,0,timer2,0,NRT);
				Timer ti3(20,20,timer3,0,NRT);
	

				bthread_set_systick_period(10);
				for(;;);
			}
			//todo timer event
			static void *timers_thread(void *)
			{
				for(;;)
				{
					btimer_run_nrt_in_thread();
				}
			}	
			
			void start()
			{
				nextThread=bthread_get_readyed();
				bthreads_start();
			}
		private:
			bThread threadFirst_;
			stack threadStackFirst_[STACK_SIZE];
			bThread threadTimer_;
			stack threadTimerStack_[STACK_SIZE];
};


}
#endif