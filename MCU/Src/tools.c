#include "tools.h"



uint64_t RAWToNumberMSBFirst(const uint8_t *data, uint8_t data_pos, uint8_t data_length){	
	if (data_length > 7) {return 0;}
	
	const uint8_t *msb_addr = data + data_pos;
	uint64_t buf = *msb_addr;
	
	for (int i = 1; i < data_length; i++){
        buf = (buf << 8) | *(msb_addr + i);
  }

  return buf;
}

uint64_t RAWToNumberLSBFirst(const uint8_t *data, uint8_t data_pos, uint8_t data_length){	
	if (data_length > 7) {return 0;}
	
	const uint8_t *msb_addr = data + data_pos + data_length - 1;
	uint64_t buf = *msb_addr;
	
	for (int i = 1; i < data_length; i++){
        buf = (buf << 8) | *(msb_addr - i);
  }

  return buf;
}

int NumberToRAWMSBFirst(uint8_t *data, uint8_t data_pos, const uint64_t number, uint8_t number_length_byte){
	if (number_length_byte > 7) {return 0;}

	uint8_t *lsb_addr = data + data_pos + number_length_byte - 1;
	
	for (int i = 0; i < number_length_byte; i++){
		*(lsb_addr - i) = (number >> (8 * i)) & 0xFF;
  }
	
	return 1;
}

int NumberToRAWLSBFirst(uint8_t *data, uint8_t data_pos, const uint64_t number, uint8_t number_length_byte){
	if (number_length_byte > 7) {return 0;}

	uint8_t *msb_addr = data + data_pos;
	
	for (int i = 0; i < number_length_byte; i++){
		*(msb_addr + i) = (number >> (8 * i)) & 0xFF;
  }
	
	return 1;
}
