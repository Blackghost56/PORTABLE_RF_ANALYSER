#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define DATA_TYPE uint8_t

struct CList{
	DATA_TYPE *data;
	struct CList *next;
};;

struct CQueue {
	struct CList *front;
	struct CList *back;
	int max_size;
	int current_size;
};

void CQueueInit(struct CQueue *queue, int max_size);
int CQueuePush(struct CQueue *queue, DATA_TYPE *data, int data_size);
int CQueuePop(struct CQueue *queue, DATA_TYPE* data, int data_size);

#endif
