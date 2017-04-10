#include "bMailBox.h"

void bmailbox_init(bMailBox *mbox,uint32_t maxCount,messageType *msgs)
{
	mbox->count_=0;
	mbox->maxCount_=maxCount;
	mbox->writeIndex_=0;
	mbox->readIndex_=0;
	mbox->msgs_=msgs;
	bmutexlock_init(&mbox->lock_);
	bcondition_init(&mbox->noEmtpy_,&mbox->lock_);
}

messageType bmaibox_get_msg(bMailBox *mbox)
{
	messageType msg=0;
	bmutexlock_lock(&mbox->lock_);
	while(mbox->count_<=0)
	{
		bcondition_wait(&mbox->noEmtpy_);
	}
	mbox->count_--;
	msg=mbox->msgs_[mbox->readIndex_++];
	if(mbox->readIndex_>=mbox->maxCount_)
	{
		mbox->readIndex_=0;
	}
	bmutexlock_unlock(&mbox->lock_);
	return msg;
}

void bmail_send_msg(bMailBox *mbox,messageType msg)
{
	bmutexlock_lock(&mbox->lock_);
	if(mbox->count_>=mbox->maxCount_)
	{
		bmutexlock_unlock(&mbox->lock_);
		return;
	}
	mbox->count_++;
	mbox->msgs_[mbox->writeIndex_++]=msg;
	if(mbox->writeIndex_>=mbox->maxCount_)
	{
		mbox->writeIndex_=0;
	}
	bcondition_notify(&mbox->noEmtpy_);
	bmutexlock_unlock(&mbox->lock_);
}
