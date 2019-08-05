#include "queue_array.h"

static struct CList* CListInit(DATA_TYPE *data) 
{
	struct CList *list;

	list = (struct CList*)malloc(sizeof(struct CList));
	list->data = data;
	list->next = NULL; 
	return list;
}

static struct CList* CListAdd(struct CList *list, DATA_TYPE *data) {
	struct CList *new_buf;
	new_buf = (struct CList*)malloc(sizeof(struct CList));
	new_buf->data = data;
	new_buf->next = list->next;
	list->next = new_buf;				
	return new_buf;
}

static struct CList* CListDeleateRoot(struct CList *list) {
	struct CList *new_root;
	new_root = list->next;
	free(list->data);
	free(list);
	return new_root;
}

void CQueueInit(struct CQueue *queue, int max_size) {
	queue->front = NULL;
	queue->back = NULL;
	queue->current_size = 0;
	queue->max_size = max_size;
}

int CQueuePush(struct CQueue *queue, DATA_TYPE *data, int data_size) {
	int state = 0;
	if (queue->current_size >= queue->max_size) {
		return state;
	}
	state = 1;
	queue->current_size++;
	struct CList *list_buf;
	DATA_TYPE *data_buf = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * data_size);
	memcpy(data_buf, data, sizeof(DATA_TYPE) * data_size);
	if (queue->front == NULL) {
		list_buf = CListInit(data_buf);
		queue->front = list_buf;
	}
	else {
		list_buf = CListAdd(queue->back, data_buf);
	}
	queue->back = list_buf;
	return state;
}

 int CQueuePop(struct CQueue *queue, DATA_TYPE* data, int data_size) {
	int state;
	state = 0;
	if (queue->front != NULL) {
		state = 1;
		memcpy(data, queue->front->data, sizeof(DATA_TYPE) * data_size);
		queue->front = CListDeleateRoot(queue->front);
		queue->current_size--;
	}
	return state;
}
