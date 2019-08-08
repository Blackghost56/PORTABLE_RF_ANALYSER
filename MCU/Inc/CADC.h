#ifndef __CADC_H
#define __CADC_H

#include "stdint.h"

#define ADC_BUF_SIZE_DEFAULT 512
#define ADC_MAX_BUF_SIZE 1024
#define ADC_MIN_BUF_SIZE  3
#define ADC_WHATCHDOG_TIMEOUT 100		// ms

#define STATE_OFF 0
#define STATE_ON  1

struct CustomADCStruct{
//volatile uint32_t ADCDataBuf[ADCBufSize];
//uint32_t ADCDataBuf[ADCBufSize];
	//uint16_t 	data[ADCBufSize];
	uint16_t 	*data;
	uint8_t 	convCompltFlag;
	uint8_t 	state;
	uint16_t 	startTime;
	//uint8_t		triggerEvent;
};



/*#include "stdlib.h"
#include "stdint.h"

#define DATA_TYPE uint8_t

struct CList{
	DATA_TYPE data;
	struct CList *next;
};

struct CQueue {
	struct CList *front;
	struct CList *back;
	int max_size;
	int current_size;
};

void CQueueInit(struct CQueue *queue, int max_size);
void CQueuePush(struct CQueue *queue, DATA_TYPE data, int *state);
void CQueuePushBuf(struct CQueue *queue, DATA_TYPE data[], int data_length, int *state);
DATA_TYPE CQueuePop(struct CQueue *queue, int *state);
void CQueuePopBuf(struct CQueue *queue, DATA_TYPE data[], int data_length, int *state);*/

#endif
