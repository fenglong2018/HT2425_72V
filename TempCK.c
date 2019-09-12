/*
 * TempCK.c
 *
 *  Created on: 2019年5月31日
 *      Author: fenglong
 */


#include "TempCK.h"
#include "system.h"

extern _BatteryData BatteryData;
extern union FW_PT FW_PT_Status;
//extern union SW_ALARM ALARM_PT_Status;        //警告值状态

void Temp_CHG_Start_Check(void)                     // started charging temperature protection
{

    if (BatteryData.TS1.TS1Word >= PT_UTSCP_Default)     //greater than 3 degrees      fenglong 20190607
    {
        FW_PT_Status.Bits.PT_UTSCP=1;
       // FW_PT_Status.Bits.PT_UTCP = 1;
//        ALARM_PT_Status.Bits.AL_GLINT=1;                    //fenglong 20190702 报警闪烁
    }
    else if(BatteryData.TS1.TS1Word < PT_UTSCP_Release)     //less than 2 degrees
    {
        FW_PT_Status.Bits.PT_UTSCP=0;
    }

    if (BatteryData.TS1.TS1Word <= PT_OTSCP_Default)     //greater than 46 degrees
    {
        FW_PT_Status.Bits.PT_OTSCP=1;
//        FW_PT_Status.Bits.PT_OTCP = 1;
//        ALARM_PT_Status.Bits.AL_GLINT=1;                    //fenglong 20190702 报警闪烁
    }
    else if(BatteryData.TS1.TS1Word > PT_OTSCP_Release)     //less than 45 degrees        fenglong 20190607
    {
        FW_PT_Status.Bits.PT_OTSCP=0;
    }
}




void Temp_DSG_Start_Check(void)                         // started discharging temperature protection
{

    if (BatteryData.TS1.TS1Word < PT_OTSDP_Default)     //greater than 61 degrees
    {
        FW_PT_Status.Bits.PT_OTSDP=1;
//        ALARM_PT_Status.Bits.AL_GLINT=1;                    //fenglong 20190702 报警闪烁

    }
    else if(BatteryData.TS1.TS1Word > PT_OTSDP_Release)     //less than 60 degrees
    {
        FW_PT_Status.Bits.PT_OTSDP=0;

    }

//    if(FW_PT_Status.Bits.PT_OTSDP == 1)
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }

}















