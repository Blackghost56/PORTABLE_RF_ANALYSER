#ifndef __CUSB_H
#define __CUSB_H

#define USBBufTxSize 2048
#define USBBufRxQSize 10


#define USB_0_CMD 			0xAA
#define USB_0_DATA 			0x55
#define USB_0_INFO 			0x95
#define USB_0_ERROR 		0xA5

#define USB_1_CMD_ADC_CONTCONV_START 						0x00
#define USB_1_CMD_ADC_CONTCONV_STOP 						0x01
#define USB_1_CMD_ADC_REQUEST_BUF_SIZE 					0x02

#define USB_1_DATA_ADC_BUF_SIZE 								0x00
#define USB_1_DATA_ADC_CONTCONV 								0x01

#define USB_1_INFO_RX_FIFO_CLEAR								0x00
#define USB_1_INFO_ADC_SET_BUF_SIZE_OK					0x01

#define USB_1_ERROR_ADC_CONTCONV_START					0x00
#define USB_1_ERROR_ADC_CONTCONV_STOP						0x01
#define USB_1_ERROR_ADC_SET_BUF_SIZE_ADCRUN			0x02
#define USB_1_ERROR_ADC_SET_BUF_SIZE_UPPERLIMIT	0x03
#define USB_1_ERROR_ADC_SET_BUF_SIZE_LOWERLIMIT	0x04
#define USB_1_ERROR_USB_FIFO_OVERLOAD 					0x05
#define USB_1_ERROR_COMMON							 				0x06






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
