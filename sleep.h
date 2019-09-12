/*
 * sleep.h
 *
 *  Created on: 2019Äê5ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef SOURCE_SLEEP_H_
#define SOURCE_SLEEP_H_

#include "system.h"


#define SleepCurrent    500//0xFFFF-1000//0x8020



void IO_SLEEP(void);
void IO_WAKEUP(void);
void SleepMode(void);
void SleepCheck(void);
void sleep_lmp0(void);
void sleep_lmp3();
void ShutDownMode();
void Clr_SCD_OCD();
#endif /* SOURCE_SLEEP_H_ */
