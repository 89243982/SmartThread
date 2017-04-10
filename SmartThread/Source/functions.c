#include "functions.h"
int i1=1;
int i2=1;
int i3=1;
int i4=1;
int i5=1;
int i6=0;
int i7=0;
int i8=0;
void delay()
	{
		int i=0;
		for(;i<0xffff;++i);
	}


void * fun1(void *a)
{
	for(;;)
	{
		i1++;
	  delay();
	}
	return 0;
}

void * fun2(void *a)
{

	for(;;)
	{
		i2++;
		delay();
	}
	return 0;
}

void * fun3(void *a)
{

	for(;;)
	{
		i3++;
		delay();
		
	}
	return 0;
}

void *fun4(void *a)
{

	i4++;
	return 0;
}

void *fun5(void *a)
{

	i5++;
	return 0;
}

void *timer1(void *a)
{
	i6++;
	return 0;
}

void *timer2(void *a)
{
	i7++;
	return 0;
}


void *timer3(void *a)
{
	i8++;
	return 0;
}