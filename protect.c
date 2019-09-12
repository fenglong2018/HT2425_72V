/*
 * protect.c
 *
 *  Created on: 2019年5月9日
 *      Author: 20190211012
 */

#include "protect.h"

#include "system.h"


extern UINT8 PT_ODP_Count;
extern UINT8 PT_OCP_Count;
//extern UINT8 TEST_3;
//extern union C1 Battery;
//extern UINT16 BatteryModeChangeDelayCount;

//extern UINT8 BatteryCapacityRefreshCount;
//extern UINT16 DC_IN_Count;
//extern UINT16 DC_IN_Release_Count;
//extern UINT16 SleepCount;
//extern UINT16 SysSHIPCount;
//extern UINT16 PT_UV_Count;


extern UINT16 CellMiniVoltage;
extern UINT16 CellMAXiVoltage;

//extern UINT8 BatteryCapacity;

extern UINT8 PF_OCC_Count;
extern UINT8 PF_OTC_Count;
extern UINT8 PF_OTM_Count;
extern UINT8 PF_OCD_Count;


//extern UINT16 PF_OTM_Count;                                         //permanent fail safety over  temperature MOSFET
extern UINT8 PT_UV_Count;
extern UINT8 PT_OV_Count;
extern UINT8 PT_UTDP_Count;
extern UINT8 PT_OTDP_Count;

extern UINT8 Cell_PF_OV_Count;
extern UINT8 Cell_PF_UV_Count;

//extern UINT16 AL_OTDA_Count;
//extern UINT16 AL_UTDA_Count;
//extern UINT16 AL_OTMA_Count;
//extern UINT16 PT_ODP_AutoReleaseCount;
//extern UINT16 HW_PT_OCD_SCD_AutoReleaseCount;

extern RegisterGroup   BQ76930Data;
extern _BatteryData    BatteryData;
//extern _FlashData      BQ76930ProtectData;
//extern union C1 Battery;
//extern union _DataSource     BQ76930ProtectDataSource;
extern union FW_PF FW_PF_Status;
extern union HW_PT HW_PT_Status;
extern union FW_PT FW_PT_Status;

extern UINT8 SystemMode;

//extern UINT8 T0_INT_count;

UINT16  Imbalance_Count=0;
extern UINT16 SW_PressCount;
//extern UINT16 SW_ReleaseCount;
//extern UINT8 DC_IN_Count;
extern UINT8 DC_IN_Release_Count;
extern UINT8 PT_OTCP_Count;                                        //over temperature charge protection
//extern UINT8 PT_OTDP_Count;                                        //over temperature discharge protection
extern UINT8 PT_UTCP_Count;                                        //under temperature charge protection
extern union SW_ALARM ALARM_PT_Status;        //警告值状态
extern UINT8 PT_OTM_Count;



void ProtectDetect(void)
{

    if (SW_PressCount >= TA0_8s)        //Engineer Mode
    {
        CLR_PF();           //fenglong 20190708
    }


//fenglong 20190704  低压后需要充电到3.4V才能动作
    if ((CellMiniVoltage >= PT_UV_Release) && (FW_PT_Status.Bits.PT_UV == 1) && (SystemMode == SystemChargeMode) )
    {
        FW_PT_Status.Bits.PT_UV = 0;
        PT_UV_Count = 0;

    }

    //================PT Detect=======================

    //1111111111111111111111111111
    //因时间短先进SHIPPING,导致无法进PF_UV，故时间有2S延长到6s   20190704 fenglong
	if (PT_UV_Count >= TA0_2s)
	{
		FW_PT_Status.Bits.PT_UV = 1;
		if (SystemMode != SystemChargeMode)
		{
			SystemMode = SystemShipMode;
		}

	}
	if (PT_OV_Count >= TA0_2s)
	{
		FW_PT_Status.Bits.PT_OV = 1;

	}
	if((FW_PT_Status.Bits.PT_OV == 1) && (CellMAXiVoltage <= PT_OV_Release))
	{
		FW_PT_Status.Bits.PT_OV =0;
		PT_OV_Count =0;
	}

	if (PT_OCP_Count >= TA0_2s)
		FW_PT_Status.Bits.PT_OCP = 1;       //over charge current protection

	//over charge current protection
	if ((FW_PT_Status.Bits.PT_OCP == 1) && (DC_IN_Release_Count >= TA0_2s))
	{
		FW_PT_Status.Bits.PT_OCP = 0;       //over charge current protection
		PT_OCP_Count = 0;
	}

	if (PT_ODP_Count >= TA0_6s)
		FW_PT_Status.Bits.PT_ODP = 1;       //over discharge current protection
	//20190704
//    if ((FW_PT_Status.Bits.PT_ODP == 1) && (PT_ODP_AutoReleaseCount >= TA0_10s))        //fenglong 20190607
//    {
//        FW_PT_Status.Bits.PT_ODP = 0;       //over discharge current protection
//        PT_ODP_Count = 0;
//        PT_ODP_AutoReleaseCount = 0;
//    }

	if ( (PT_OTCP_Count >= TA0_2s) && (SystemMode == SystemChargeMode) )     //fenglong 20190705
		FW_PT_Status.Bits.PT_OTCP = 1;                          //over temperature charge protection

	if ((FW_PT_Status.Bits.PT_OTCP == 1)                        //over temperature charge protection
			&& (BatteryData.TS1.TS1Word >= PT_OTCP_Release))
	{
		FW_PT_Status.Bits.PT_OTCP = 0;
		FW_PT_Status.Bits.PT_OTSCP=0;
		PT_OTCP_Count = 0;
	}



	if ( (PT_UTCP_Count >= TA0_2s) && (SystemMode == SystemChargeMode) )    //fenglong 20190705
	{
		FW_PT_Status.Bits.PT_UTCP = 1;                      //under temperature charge protection
	}


	if ((FW_PT_Status.Bits.PT_UTCP == 1)
			&& (BatteryData.TS1.TS1Word <= PT_UTCP_Release))
	{
		FW_PT_Status.Bits.PT_UTCP = 0;                      //under temperature charge protection
		FW_PT_Status.Bits.PT_UTSCP=0;
		PT_UTCP_Count = 0;
	}

	if (PT_OTDP_Count >= TA0_2s)
		FW_PT_Status.Bits.PT_OTDP = 1;                      //over temperature discharge protection

	if ((FW_PT_Status.Bits.PT_OTDP == 1)                    //over temperature discharge protection
			&& (BatteryData.TS1.TS1Word >= PT_OTDP_Release))
	{
		FW_PT_Status.Bits.PT_OTDP = 0;                      //over temperature discharge protection
		PT_OTDP_Count = 0;
	}

	if (PT_UTDP_Count >= TA0_2s)
		FW_PT_Status.Bits.PT_UTDP = 1;          //under temperature discharge protection

	if ((FW_PT_Status.Bits.PT_UTDP == 1)
			&& (BatteryData.TS1.TS1Word <= PT_UTDP_Release))
	{
		FW_PT_Status.Bits.PT_UTDP = 0;          //under temperature discharge protection
		PT_UTDP_Count = 0;
	}

	if(PT_OTM_Count >= PT_OTM_Delay2S)
	{
		ALARM_PT_Status.Bits.HW_OTM=1;
	}
	if( (ALARM_PT_Status.Bits.HW_OTM == 1) && (BatteryData.TS2.TS2Word >= PT_OTM_Release) )
	{
		ALARM_PT_Status.Bits.HW_OTM=0;
		PT_OTM_Count=0;
	}


//=============PF Detect==============================
	if (Cell_PF_UV_Count >= TA0_8s)
	{
		FW_PF_Status.Bits.PF_UV = 1;            //permanent fail safety under voltage

	}

	if (Cell_PF_OV_Count >= TA0_8s)
	{
		FW_PF_Status.Bits.PF_OV = 1;            //4.23mV    -   8s      //permanent fail safety over voltage
	}

	if (PF_OCC_Count >= TA0_2s)
	{
		FW_PF_Status.Bits.PF_OCC = 1;           //permanent fail  over current in charge
	}

	if (PF_OCD_Count >= TA0_4s)                 //permanent fail  over current in discharge
	{
		FW_PF_Status.Bits.PF_OCD = 1;           //fenglong 20190607
	}

	if (PF_OTC_Count >= TA0_4s)
	{
		FW_PF_Status.Bits.PF_OTC = 1;
	}

	if (PF_OTM_Count >= TA0_4s)                 //permanent fail safety over  temperature MOSFET
	{
		FW_PF_Status.Bits.PF_OTM = 1;
	}

//=============BQ76930 PT====================
			//cell over voltage 单体过压



//    result = I2C_Load_BQ76930(&(BQ76930Data.SysStatus.StatusByte), SYS_STAT, 1);
    if(I2C_Load_BQ76930(&(BQ76930Data.SysStatus.StatusByte), SYS_STAT, 1) ==0)
    {
        PT_ProtectRenew();
    }


	uchar  Buff_1;
	if ((HW_PT_Status.Bits.HW_OV == 1) && (CellMAXiVoltage <= PT_OV_Release))  // 过压保护恢复
	{
		Buff_1 = HW_PT_Status.Byte;
		//HW_PT_Status.Byte = 0x00;
		//HW_PT_Status.Bits.HW_OV = 1;        //cell over voltage 单体过压
		BQ76930Data.SysStatus.StatusByte =0x04;
		//BQ76930Data.SysStatus.StatusBit.OV = 1;
//		Buff_0 = I2C_Master_TransmitByte(BQ76930_SysState_Addr,BQ76930Data.SysStatus.StatusByte);

		if (I2C_Master_TransmitByte(BQ76930_SysState_Addr,BQ76930Data.SysStatus.StatusByte) == 0)
		{
			HW_PT_Status.Byte = Buff_1;
			HW_PT_Status.Bits.HW_OV = 0;            //cell over voltage 单体过压

			BQ76930Data.SysStatus.StatusBit.OV =0;
		}
	}
			//cell under voltage 单节欠压恢复
	if ((HW_PT_Status.Bits.HW_UV == 1) && (CellMiniVoltage >= PT_UV_Release)
			&& (SystemMode == SystemChargeMode))
	{
		Buff_1 = HW_PT_Status.Byte;
		//HW_PT_Status.Byte = 0x00;
		//HW_PT_Status.Bits.HW_UV = 1;        // cell under voltage
		BQ76930Data.SysStatus.StatusByte =0;
		BQ76930Data.SysStatus.StatusBit.UV = 1;
//		Buff_0 = I2C_Master_TransmitByte(BQ76930_SysState_Addr,BQ76930Data.SysStatus.StatusByte);

		if (I2C_Master_TransmitByte(BQ76930_SysState_Addr,BQ76930Data.SysStatus.StatusByte) == 0)
		{
			HW_PT_Status.Byte = Buff_1;
			HW_PT_Status.Bits.HW_UV = 0;            //cell under voltage 单体过流
			BQ76930Data.SysStatus.StatusBit.UV =0;
		}
	}

}




void ImbalanceCheck(void)
{


	if(Imbalance_Count >= (PF_VIMA_Delay220S) )
	{
		FW_PF_Status.Bits.PF_VIMA=1;
	}


}


//void PortectVBat(void)
//{
//
////    BatteryData.VBat.VBatWord;
//
//
//
//}
