#ifndef _CTRL_H
#define _CTRL_H
#include "stm32f10x.h"
void Ctrl_Init(void);
void Pwr_Init(void);
void RecordOnHint(void);
void RecordOffHint(void);
void handleCtrl(void);
void handleRecordState(void);
void handlePlayState(void);
void Pwr_Init(void);
void SleepMode(void);
typedef void StateChanged(u8 nowState);
typedef struct {
	u8 lastRecordValue;
	u8 lastPlayValue;
	StateChanged* recordStateChanged;
	StateChanged* playStateChanged;
}_ctrl;
extern _ctrl ctrl;
#endif
