#ifndef _ADPCM_H
#define _ADPCM_H
#include "stm32f10x.h"
uint8_t ADPCM_Encode(int32_t sample);
int16_t ADPCM_Decode(uint8_t code);
void Adpcm_FrameEncode(const int16_t *PCMBuffer, void *EncodeBuffer, int32_t Len);
void Adpcm_FrameDecode(int16_t *PCMBuffer, const void *EncodeBuffer, int32_t Len);


#endif
