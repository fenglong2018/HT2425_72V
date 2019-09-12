/*
 * TempCK.h
 *
 *  Created on: 2019Äê5ÔÂ31ÈÕ
 *      Author: fenglong
 */

#ifndef TEMPCK_H_
#define TEMPCK_H_

#include "system.h"


#define PT_OTSCP_Default        2728     //46 Deg            //over temperature before start charge protection      fenglong 20190607
#define PT_OTSCP_Release        2791   //45 Deg                                                                     fenglong 20190607

#define PT_OTSDP_Default         1928   //61 Deg            //over temperature before start discharge protection     fenglong 20190607
#define PT_OTSDP_Release         1970   //60 Deg                                                                    fenglong 20190607

#define PT_UTSCP_Default         6237   //2 Deg             //under temperature before start charge protection                              fenglong 20190607
#define PT_UTSCP_Release         6167   //3 Deg                                                                                             fenglong 20190607

//#define PT_UTSDP_Default         7622      //3900 = 7791    //-18 Deg           //under temperature before start discharge protection
//#define PT_UTSDP_Release         7349  //27   3900 = 7400       //-13 Deg






void Temp_CHG_Start_Check(void);
void Temp_CHG_End_Check(void);
void Temp_CHG_End(void);

void Temp_DSG_Start_Check(void);


#endif /* TEMPCK_H_ */
