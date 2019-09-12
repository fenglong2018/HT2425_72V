#ifndef __INITIAL_H__
#define __INITIAL_H__

#include "system.h"

void UART_Initial (void);
void ConfigWDT(void);
void SystemInitial (void);
void RAM_Init(void);
void BlackBox_Init(void);
void BQ_Init(void);
void Get_Gain(void);
void his_data_init(void);
void initClockTo12MHz();
void GPIOInitial(void);
void Timer_A_initial(void);
void I2C_Initial(void);


#endif
