/*
 * count.c
 *
 *  Created on: 2019年8月21日
 *      Author: fenglong
 */

#include "count.h"
#include "System.h"

extern union _SW SW_Status;
extern union FW_PT FW_PT_Status;
extern _BatteryData    BatteryData;
//extern UINT8 SysRunCount;
UINT8 BatteryCapacityRefreshCount = 0;

UINT16 SW_PressCount=0;
UINT16 SW_ReleaseCount=0;

extern UINT8 DC_IN_Count;
extern UINT8 DC_IN_Release_Count;

extern UINT8 SysSHIPCount;

extern UINT8 SystemMode;


extern UINT16 CellMiniVoltage;
extern UINT16 CellMAXiVoltage;

extern UINT8 Cell_PF_OV_Count;
extern UINT8 Cell_PF_UV_Count;
extern UINT8 PF_OCC_Count;
extern UINT8 PF_OTC_Count;
extern UINT8 PF_OTM_Count;
extern UINT8 PF_OCD_Count;

extern UINT8 PT_OTCP_Count;                                        //over temperature charge protection
extern UINT8 PT_OTDP_Count;                                        //over temperature discharge protection
extern UINT8 PT_UTCP_Count;                                        //under temperature charge protection
extern UINT8 PT_UTDP_Count;
extern UINT8 PT_UV_Count;
extern UINT8 PT_OV_Count;
extern UINT8 PT_OCP_Count;                                         //over charge current protection
extern UINT8 PT_ODP_Count;                                         //over discharge current protection

extern  UINT8 Sleep_delay;
extern UINT8 SleepCount;
UINT8 PT_OTM_Count=0;
extern UINT16  Imbalance_Count;

void key_count(void)
{
    if(KEY_DOWN)
    {
        if(SW_Status.bits.SW_Action == 1)
        {
            SW_Status.bits.SW_CKSOC=0;              //fenglong 20190621
        }
        if(SW_PressCount<TA0_15000ms)
        {
            SW_PressCount = SW_PressCount + 1;
        }
        SW_ReleaseCount=0;
    }
    else
    {
        if(SW_ReleaseCount < TA0_15000ms)
        {
            SW_ReleaseCount++;
        }
        SW_PressCount=0;
    }
}

void DelayTime200ms_count(void)
{

//	if(SystemMode == SystemDischargeMode)
//	{
//		if(SysRunCount < 50)
//		{
//			SysRunCount++;
//		}
//	}

    if( SystemMode == SystemChargeMode )
    {
    	if(   (FW_PT_Status.Bits.PT_OTSCP == 1)
    		||(FW_PT_Status.Bits.PT_UTSCP == 1)
    	  )
    	{
    		DC_IN_Count = 0;
    	}
    }

    if (DC_IN)                 //DC in  ,charge
    {
        SysSHIPCount =0;
        if (DC_IN_Count < TA0_5s)      //cancel =   fenglong 20190607
        {
            DC_IN_Count++;
            if (DC_IN_Count < TA0_3s)
            {
                Temp_CHG_Start_Check();
            }
        }
        DC_IN_Release_Count = 0;
    }
    else                                //DC pull out
    {
        DC_IN_Count = 0;
        if (DC_IN_Release_Count <= TA0_5s)
        {
            DC_IN_Release_Count = DC_IN_Release_Count + 1;
        }

    }

    if (PT_UV_Count <= TA0_10s)
    {
        if (PT_UV_Default >= CellMiniVoltage)
            PT_UV_Count = PT_UV_Count + 1;
        else
            PT_UV_Count = 0;
    }
    if (PT_OV_Count <= TA0_10s)
    {
        if (PT_OV_Default < CellMAXiVoltage)
            PT_OV_Count = PT_OV_Count + 1;
        else
            PT_OV_Count = 0;
    }

    if (Cell_PF_UV_Count <= TA0_10s)
    {
        if (PF_UV_Default > CellMiniVoltage)
            Cell_PF_UV_Count = Cell_PF_UV_Count + 1;
        else
            Cell_PF_UV_Count = 0;           //permanent fail safety under voltage
    }

    if (Cell_PF_OV_Count <= TA0_10s)
    {
        if (CellMAXiVoltage > PF_OV_Default)
            Cell_PF_OV_Count = Cell_PF_OV_Count + 1;
        else
            Cell_PF_OV_Count = 0;           //4.23mV    -   8s      //permanent fail safety over voltage
    }

    if (PF_OTC_Count <= TA0_4s)
    {
        if (BatteryData.TS1.TS1Word < PF_OTC_Default)
            PF_OTC_Count = PF_OTC_Count + 1;
        else
            PF_OTC_Count = 0;
    }

    if (PF_OTM_Count <= TA0_8s)
    {
        if (BatteryData.TS2.TS2Word < PF_OTM_Default)
            PF_OTM_Count = PF_OTM_Count + 1;
        else
            PF_OTM_Count = 0;                   //permanent fail safety over  temperature MOSFET
    }


    if (PT_OTDP_Count <= TA0_8s)                            //over temperature discharge protection
    {
        if (BatteryData.TS1.TS1Word <= PT_OTDP_Default)
            PT_OTDP_Count = PT_OTDP_Count + 1;
        else
            PT_OTDP_Count = 0;
    }

    if (PT_UTDP_Count <= TA0_8s)                        //under temperature discharge protection
    {
        if (BatteryData.TS1.TS1Word >= PT_UTDP_Default)
            PT_UTDP_Count = PT_UTDP_Count + 1;
        else
            PT_UTDP_Count = 0;
    }

    if (PT_OTCP_Count <= TA0_8s)                //over temperature charge protection
    {
        if (BatteryData.TS1.TS1Word <= PT_OTCP_Default)     //fenglong 20190607
            PT_OTCP_Count = PT_OTCP_Count + 1;
        else
            PT_OTCP_Count = 0;
    }

    if (PT_UTCP_Count <= TA0_8s)
    {
        if (BatteryData.TS1.TS1Word >= PT_UTCP_Default)
            PT_UTCP_Count = PT_UTCP_Count + 1;
        else
            PT_UTCP_Count = 0;                              //under temperature charge protection
    }

    //過流保护
    if (BatteryData.CC.CCWord >= 0x8000)        //库仑计   coulomb counter
    {
        UINT16 cur_tmp;

    	PF_OCC_Count = 0;
    	PT_OCP_Count = 0;
    	cur_tmp = 0-BatteryData.CC.CCWord;
        if (PF_OCD_Count <= TA0_8s)             //permanent fail  over current in discharge
        {
            if ((cur_tmp) > PF_OCD_Default) //放电 50A
                PF_OCD_Count = PF_OCD_Count + 1;
            else
                PF_OCD_Count = 0;
        }
        if (PT_ODP_Count <= TA0_8s)             //over discharge current protection
        {

            if ((cur_tmp) > PT_ODP_Default) //放电 25A
                PT_ODP_Count = PT_ODP_Count + 1;
            else
                PT_ODP_Count = 0;
        }
    }
    else
    {
    	PT_ODP_Count = 0;
    	PF_OCD_Count = 0;
        //充电 过流
        if (PF_OCC_Count <= TA0_8s)
        {
            if ((BatteryData.CC.CCWord > PF_OCC_Default))   //6A       //permanent fail  over current in charge
                PF_OCC_Count = PF_OCC_Count + 1;
            else
                PF_OCC_Count = 0;
        }

        if (PT_OCP_Count <= TA0_8s)
        {
            if (BatteryData.CC.CCWord > PT_OCP_Default)  //2.6A
                PT_OCP_Count = PT_OCP_Count + 1;
            else
                PT_OCP_Count = 0;                       //over charge current protection
        }
    }

	if( (SystemMode==SystemSleepMode) && (Sleep_delay<=TA0_20s) )
	{
		Sleep_delay++;
	}

    if (PT_OTM_Count <= TA0_8s)
    {
        if (BatteryData.TS2.TS2Word < PT_OTM_Default)
            PT_OTM_Count = PT_OTM_Count + 1;
        else
            PT_OTM_Count = 0;                   //over  temperature MOSFET
    }


//        if(SleepCount < TA0_21m)
//        {
//            SleepCount++;
//        }

    //    if(SysRunCount<TA0_1m)
    //        SysRunCount++;

//20190704
//    if (FW_PT_Status.Bits.PT_ODP == 1)          //over discharge current protection
//        PT_ODP_AutoReleaseCount = PT_ODP_AutoReleaseCount + 1;
//                //放电过流2级保护                                                  //short current detection  短路监测电流

//    if ((HW_PT_Status.Bits.HW_OCD == 1) || (HW_PT_Status.Bits.HW_SCD))			//20190704 客户要求取消
//	{
//    	HW_PT_OCD_SCD_AutoReleaseCount = HW_PT_OCD_SCD_AutoReleaseCount + 1;	//20190704 客户要求取消
//	}

}


void DelayTime1s_count(void)
{
	if (BatteryCapacityRefreshCount <= TA0_20s)
	{
		BatteryCapacityRefreshCount++;
	}

    if(SleepCount < TA0_21m)
    {
        SleepCount++;
    }

	if(Imbalance_Count <= TA0_300s )
	{
		if( ( (CellMAXiVoltage - CellMiniVoltage) >=700 ) && (CellMAXiVoltage>=3700) )
		{
			Imbalance_Count++;
		}
		else
		{
			Imbalance_Count=0;

		}
	}


}


//void DelayTime1m_count(void)
//{
//
//    if(SleepCount < TA0_21m)
//    {
//        SleepCount++;
//    }
//
//}














