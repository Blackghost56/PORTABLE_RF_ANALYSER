#include "queue.h"

static struct CList* CListInit(DATA_TYPE data) 
{
	struct CList *list;

	list = (struct CList*)malloc(sizeof(struct CList));
	list->data = data;
	list->next = NULL; 
	return list;
}

static struct CList* CListAdd(struct CList *list, DATA_TYPE data) {
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
	free(list);
	return new_root;
}

void CQueueInit(struct CQueue *queue, int max_size) {
	queue->front = NULL;
	queue->back = NULL;
	queue->current_size = 0;
	queue->max_size = max_size;
}

void CQueuePush(struct CQueue *queue, DATA_TYPE data, int *state) {
	if (queue->current_size >= queue->max_size) {
		*state = 0;
		return;
	}
	*state = 1;
	queue->current_size++;
	struct CList *list_buf;
	if (queue->front == NULL) {
		list_buf = CListInit(data);
		queue->front = list_buf;
		queue->back = list_buf;
	}
	else {
		list_buf = CListAdd(queue->back, data);
		queue->back = list_buf;
	}
}

static void CQueuePush2(struct CQueue *queue, DATA_TYPE data) {
	queue->current_size++;
	struct CList *list_buf;
	if (queue->front == NULL) {
		list_buf = CListInit(data);
		queue->front = list_buf;
		queue->back = list_buf;
	}
	else {
		list_buf = CListAdd(queue->back, data);
		queue->back = list_buf;
	}
}

DATA_TYPE CQueuePop(struct CQueue *queue, int *state) {
	DATA_TYPE buf = 0;
	*state = 0;
	if (queue->front != NULL) {
		*state = 1;
		buf = queue->front->data;
		queue->front = CListDeleateRoot(queue->front);
		queue->current_size--;
	}
	return buf;
}

static DATA_TYPE CQueuePop2(struct CQueue *queue) {
	DATA_TYPE buf = 0;
	if (queue->front != NULL) {
		buf = queue->front->data;
		queue->front = CListDeleateRoot(queue->front);
		queue->current_size--;
	}
	return buf;
}

void CQueuePushBuf(struct CQueue *queue, DATA_TYPE data[], int data_length, int *state) {
	if ((queue->max_size - queue->current_size) < data_length) {
		*state = 0;
		return;
	}
	*state = 1;
	for (int i = 0; i < data_length; i++) {
		CQueuePush2(queue, data[i]);
		//CQueuePush(queue, data[i], state);
	}
}

void CQueuePopBuf(struct CQueue *queue, DATA_TYPE data[], int data_length, int *state) {
	if (queue->current_size < data_length) {
		*state = 0;
		return;
	}
	*state = 1;
	for (int i = 0; i < data_length; i++) {
		data[i] = CQueuePop2(queue);
		//data[i] = CQueuePop(queue, state);
	}
}
