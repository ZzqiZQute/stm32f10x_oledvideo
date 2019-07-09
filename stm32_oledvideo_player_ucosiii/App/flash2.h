#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x.h"
#define WriteFlashAddress    ((u32)0x08010800)//Start Position
#define SoundSizeAddress ((u32)0x08010000) //u32
#define MAX_FLASH_PAGE 127
#define MAX_FLASH_SIZE 2048*MAX_FLASH_PAGE
u8 Write_Flash(u32 addr,u32 *buff, u8 len);
void Read_Flash(u32 addr,u32 *buff, u8 len);
u8 Write_Flash_At(u32 addr,u8 *buff, u32 len);
void Read_Flash_At(u32 addr,u8 *buff, u32 len);
void Reset_Flash(void);
#endif
