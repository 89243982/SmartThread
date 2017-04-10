#ifndef SMARTTHREAD_NONCOPYABLE_H
#define SMARTTHREAD_NONCOPYABLE_H
namespace smartthread
{
	class noncopyable  
	{  
		protected:  
			noncopyable() {}  
			~noncopyable() {}  
		private:  
			noncopyable( const noncopyable& );  
			noncopyable& operator=( const noncopyable& );  
	};
}

#endif