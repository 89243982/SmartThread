#ifndef SMARTTHREAD_BMAILBOX_H
#define SMARTTHREAD_BMAILBOX_H
#include "stdint.h"
#include "bTypes.h"
#include "bMutex.h"
#include "bCondition.h"
typedef struct _bMailBox
{
	uint32_t count_;
	uint32_t maxCount_;
	uint32_t writeIndex_;
	uint32_t readIndex_;
	messageType *msgs_;
	bMutexLock lock_;
	bCondition noEmtpy_;
}bMailBox;


void bmailbox_init(bMailBox *mbox,uint32_t maxCount,messageType *msgs);
messageType bmaibox_get_msg(bMailBox *mbox);
void bmail_send_msg(bMailBox *mbox,messageType msg);

#endif