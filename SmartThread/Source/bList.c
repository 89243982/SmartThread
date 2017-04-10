#include "bList.h"

void bnode_init (bNode * node)
{
	node->nextNode_= node;
	node->preNode_ = node;
}



void blist_init (bList * list)
{
	list->firstNode_= &(list->headNode_);
	list->lastNode_= &(list->headNode_);
	list->count_ = 0;
}

uint32_t blist_count(bList* list)
{
	return list->count_;
}

bNode * blist_first(bList* list)
{
	bNode * node = (bNode *)0;
	
	if (list->count_!= 0)
	{
		node = list->firstNode_;
	}
	return node;
}

bNode * blist_last (bList * list)
{
	bNode * node = (bNode *)0;
	
	if (list->count_!= 0)
	{
		node = list->lastNode_;
	}
	return node;
}

bNode * blist_pre (bList *list, bNode * node)
{
	if (node->preNode_== node)
	{
		return (bNode *)0;
	}
	else
	{
		return node->preNode_;
	}
}

bNode * blist_next (bList *list, bNode * node)
{
	if (node->nextNode_== node)
	{
		return (bNode *)0;
	}
	else
	{
		return node->nextNode_;
	}
}

void blist_remove_all (bList * list,func fun)
{
	uint32_t count;
	bNode * nextNode;
	nextNode = list->firstNode_;
	for (count =0; count<list->count_; count++)
	{
		bNode *currentNode = nextNode;
		nextNode = currentNode->nextNode_;
		bnode_init(currentNode);
		fun(currentNode);
	}
	blist_init(list);
}

void blist_add_first(bList * list, bNode * node)
{
	node->preNode_=&list->headNode_;
	node->nextNode_=list->firstNode_;
	
	list->firstNode_->preNode_ = node;
	list->firstNode_ = node;
	list->count_++;
}

void blist_add_last(bList * list, bNode * node)
{
	node->nextNode_= &(list->headNode_);
	node->preNode_= list->lastNode_;
	
	list->lastNode_->nextNode_ = node;
	list->lastNode_ = node;
	list->count_++;
}

bNode * blist_remove_first (bList * list)
{
	bNode *node = (bNode*)0;
	if (list->count_>0)
	{
		node = list->firstNode_;
		node->nextNode_->preNode_= &(list->headNode_);
		list->firstNode_= node->nextNode_;
		bnode_init(node);
		list->count_--;
	}
	return node;
}

void blist_insert_after (bList * list, bNode * nodeBefore, bNode *nodeInsert)
{
	nodeInsert->preNode_= nodeBefore;
	nodeInsert->nextNode_ = nodeBefore->nextNode_;
	
	nodeBefore->nextNode_->preNode_= nodeInsert;
	nodeBefore->nextNode_=nodeInsert;
	
	list->count_++;
}

void blist_remove (bList * list, bNode * node)
{
	node->preNode_->nextNode_ = node->nextNode_;
	node->nextNode_->preNode_ = node->preNode_;
	bnode_init(node);
	list->count_--;
}


bList threadsRdy[PRIORITY_MAX];
bList threadsDelayed;
bList timers_rt;
bList timers_nrt;


void threads_readyed_init()
{
	int i;
	for(i=0;i<PRIORITY_MAX;++i)
	{
		blist_init(&threadsRdy[i]);
	}
}
void threads_delayed_init()
{
	blist_init(&threadsDelayed);
}

void timers_init()
{
	blist_init(&timers_rt);
	blist_init(&timers_nrt);
}
void threads_all_list_init()
{
	threads_readyed_init();
	threads_delayed_init();
	timers_init();
}
