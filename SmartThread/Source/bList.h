#ifndef SMARTTHREAD_LIST_H
#define SMARTTHREAD_LIST_H
#include <stdint.h>
#include "bConfig.h"
#include "bTypes.h"

typedef struct _bNode
{
	struct _bNode * preNode_;
	struct _bNode * nextNode_;
}bNode;

typedef struct _bList
{
	bNode headNode_;
	uint32_t count_;
}bList;
#define	firstNode_ headNode_.nextNode_
#define	lastNode_ headNode_.preNode_
#define	bnode_parent(nodeAddr,parentType,varname)	(parentType*)((uint32_t)nodeAddr - (uint32_t)&((parentType *)0)->varname)

void bnode_init (bNode * node);

void blist_init (bList * list);
uint32_t blist_count(bList* list);
bNode * blist_first(bList* list);
bNode * blist_last (bList * list);
bNode * blist_pre (bList *list, bNode * node);
bNode * blist_next (bList *list, bNode * node);
void blist_remove_all (bList * list,func fun);
void blist_add_first(bList * list, bNode * node);
void blist_add_last(bList * list, bNode * node);
bNode * blist_remove_first (bList * list);
void blist_insert_after (bList * list, bNode * nodeBefore, bNode *nodeInsert);
void blist_remove (bList * list, bNode * node);
void threads_all_list_init();
extern bList threadsRdy[PRIORITY_MAX];
extern bList threadsDelayed;
extern bList timers_rt;
extern bList timers_nrt;

#endif