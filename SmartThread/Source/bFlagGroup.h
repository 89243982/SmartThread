#ifndef SMARTTHREAD_FLAGGROUP_H
#define SMARTTHREAD_FLAGGROUP_H
#include <stdint.h>
#include <stdbool.h>
#include "bEvent.h"

typedef enum
{
			ALL_SET=0,
			ALL_CLEAR,
			ANY_SET,
			ANY_CLEAR
}CheckMethod;
typedef struct _bFlagGroup
{
	uint32_t flags_;
	bEvent waitEvent_;
}bFlagGroup;


void bflaggroup_init(bFlagGroup *fg);
void bflaggroup_take_flags(bFlagGroup *fg,CheckMethod method);
void bflaggroup_set_flags(bFlagGroup *fg,uint32_t flags);

extern bFlagGroup test;

#endif