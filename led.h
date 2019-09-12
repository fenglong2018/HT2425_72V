/*
 * led.h
 *
 *  Created on: 2019��7��12��
 *      Author: fenglong
 */

#ifndef LED_H_
#define LED_H_

#include "System.h"


struct _LED_PWM_CTRL
{
    uchar PWM1_EN:       1;         //PWM����
    uchar PWM1_UP:       1;         //1������0����
//    uchar PWM1_START:    1;         //
    uchar PWM1_STOP:     1;
    uchar PWM1_HOLD:     1;

    uchar PWM2_EN:       1;         //PWM����
    uchar PWM2_UP:       1;         //1������0����
//    uchar PWM2_START:    1;         //
    uchar PWM2_STOP:     1;
    uchar PWM2_HOLD:     1;

    uchar PWM3_EN:       1;         //PWM����
    uchar PWM3_UP:       1;         //1������0����
//    uchar PWM3_START:    1;         //
    uchar PWM3_STOP:     1;
    uchar PWM3_HOLD:     1;

};

union _LED_CTRL
{
    uint Word;
    struct _LED_PWM_CTRL Bits;
};

void LED1_PWM( uchar LEDx);
void LED2_PWM( uchar LEDx);
void LED3_PWM( uchar LEDx);
void LED_OFF(void);

void Charge_Mode3_LED_Display(void);
void Charge_Mode2_LED_Display(void);

#endif /* LED_H_ */
