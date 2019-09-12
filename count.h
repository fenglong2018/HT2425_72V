/*
 * count.h
 *
 *  Created on: 2019Äê8ÔÂ21ÈÕ
 *      Author: fenglong
 */

#ifndef COUNT_H_
#define COUNT_H_

#include "System.h"


#define TA0_40ms                    2		//20ms
#define TA0_100ms                   5
#define TA0_200ms                   10
#define TA0_400ms                   20
#define TA0_1500ms                  75
#define TA0_4000ms					200
#define TA0_15000ms                 750


#define TA0_1s                      5			//200ms
#define TA0_2s                      10
#define TA0_3s                      15
#define TA0_4s                      20
#define TA0_5s                      25
#define TA0_6s                      30
#define TA0_8s                      40
#define TA0_10s                     50


#define TA0_20s                     20

#define TA0_300s                    300

#define TA0_1m                      60			//s





#define TA0_10m                     10			//m
#define TA0_15m                     15
#define TA0_20m                     20
#define TA0_21m                     21

void DelayTime200ms_count(void);
void DelayTime1s_count(void);
//void DelayTime1m_count(void);
void key_count(void);

#endif /* COUNT_H_ */
