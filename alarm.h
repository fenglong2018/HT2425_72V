/*
 * alarm.h
 *
 *  Created on: 2019Äê5ÔÂ16ÈÕ
 *      Author: fenglong
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "system.h"

#define ALARM_OV_Default        4150
#define ALARM_OV_Release        4100
#define ALARM_OV_DelayTime2S    100     //2S

#define ALARM_UV_Default        3100
#define ALARM_UV_Release        3400
#define ALARM_UV_DelayTime2S    100     //2S

#define ALARM_VBAT_OV_Default        29190          //
#define ALARM_VBAT_OV_Release        28420
#define ALARM_VBAT_OV_DelayTime2S    100     //2S

#define ALARM_VBAT_UV_Default        21700          //
#define ALARM_VBAT_UV_Release        23800
#define ALARM_VBAT_UV_DelayTime2S    100     //2S


void AlarmDetect(void);

#endif /* ALARM_H_ */
