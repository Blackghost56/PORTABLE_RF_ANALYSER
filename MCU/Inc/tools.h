#ifndef __TOOLS_H
#define __TOOLS_H

#include "stdint.h"

uint64_t RAWToNumberMSBFirst(const uint8_t *data, uint8_t data_pos, uint8_t data_length);
uint64_t RAWToNumberLSBFirst(const uint8_t *data, uint8_t data_pos, uint8_t data_length);
int NumberToRAWMSBFirst(uint8_t *data, uint8_t data_pos, const uint64_t number, uint8_t number_length_byte);
int NumberToRAWLSBFirst(uint8_t *data, uint8_t data_pos, const uint64_t number, uint8_t number_length_byte);

#endif
