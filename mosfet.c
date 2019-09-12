/*
 * mosfet.c
 *
 *  Created on: 2019年5月17日
 *      Author: fenglong
 */

#include "system.h"
#include "mosfet.h"


//UINT16 TOFF_CHG_MOS_Count=0;
//UINT16 TOFF_DHG_MOS_Count=0;
//
//
//
//extern UINT8 T0_INT_count;
//extern UINT8 MosStatus;
//extern union FW_PF FW_PF_Status;       //  三级保护？
//extern union SW_ALARM ALARM_PT_Status;        //警告值状态
//
//extern RegisterGroup BQ76930Data;
//extern _BatteryData BatteryData;



//void MosfetDetect(void)
//{
//    unsigned short cur_tmp;
//    if(T0_INT_count>0)
//    {
//        if(BQ76930Data.SysCtrl2.SysCtrl2Bit.DSG_ON ==0 )      //CHG_ON    BQ76930Data.SysCtrl2.SysCtrl2Bit.DSG_ON
//        {
//            if(BatteryData.CC.CCWord > 0x8000)
//            {
//                cur_tmp = 0-BatteryData.CC.CCWord;
//                if(cur_tmp >= TOFF_DHG_MOS_CUR )
//                {
//                    if (TOFF_DHG_MOS_Count <= TA0_1m)
//                    {
//                        TOFF_DHG_MOS_Count++;
//                    }
//                }
//                else
//                {
////                	_nop();
//                    TOFF_DHG_MOS_Count=0;//FENGLONG 20190805
//                }
//            }
//            else
//            {
////            	_nop();
//                TOFF_DHG_MOS_Count=0;//FENGLONG 20190805
//            }
//        }
//        else
//        {
//            TOFF_DHG_MOS_Count=0;
//        }

//        if(BQ76930Data.SysCtrl2.SysCtrl2Bit.CHG_ON ==0 )        //CHG_OFF
//        {
//            cur_tmp = BatteryData.CC.CCWord;
//            if(cur_tmp >= TOFF_CHG_MOS_CUR )
//            {
//                if (TOFF_CHG_MOS_Count <= TA0_1m)
//                {
//                    TOFF_CHG_MOS_Count++;
//                }
//            }
//            else
//            {
//                TOFF_CHG_MOS_Count=0;
//            }
//        }
//        else
//        {
//            TOFF_CHG_MOS_Count=0;
//        }

//        if (TOFF_CHG_MOS_Count >= ATOFF_CHG_MOS_DelayTime20S)
//        {
//            FW_PF_Status.Bits.PF_CMF = 1 ;
//        }
//        if (TOFF_DHG_MOS_Count >= ATOFF_DHG_MOS_DelayTime20S)
//        {
//            FW_PF_Status.Bits.PF_DMF = 1 ;
//        }





//    }
//}
