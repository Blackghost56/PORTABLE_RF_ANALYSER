#ifndef __CADC_H
#define __CADC_H

#include "stdint.h"

#define ADC_BUF_SIZE_DEFAULT 200
#define ADC_MAX_BUF_SIZE 16384
#define ADC_MIN_BUF_SIZE  3
#define ADC_WHATCHDOG_TIMEOUT 100		// ms

#define STATE_OFF 0
#define STATE_ON  1

struct CustomADCStruct{
	uint16_t 	*data;
	uint8_t 	convCompltFlag;
	uint8_t 	state;
	uint16_t 	startTime;
	//uint8_t		triggerEvent;
};


#endif
