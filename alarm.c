/*
 * alarm.c
 *
 *  Created on: 2019��5��16��
 *      Author: fenglong
 */

#include "system.h"
#include "alarm.h"


//extern UINT16 CellMiniVoltage ;
//extern UINT16 CellMAXiVoltage ;

//extern UINT8 T0_INT_count;

extern union FW_PF FW_PF_Status;       //  ����������

extern union HW_PT HW_PT_Status;       //BQ76930       ��������
extern union FW_PT FW_PT_Status;       //CPU           һ������

//extern _BatteryData BatteryData;
//extern _BatteryRealData    BatteryRealData;

union SW_ALARM ALARM_PT_Status;        //����ֵ״̬

UINT16 ALARM_OV_Count=0;
UINT16 ALARM_UV_Count=0;

UINT16 ALARM_UV_VBAT_Count=0;
UINT16 ALARM_OV_VBAT_Count=0;

extern UINT8 SystemMode;
//extern uchar Alert_count;

void AlarmDetect(void)
{

//	if(ALARM_UV_VBAT_Count <= TA0_10s )        //OV_VBAT �����Ƿѹ
//	{
//		if (ALARM_VBAT_UV_Default >= BatteryData.VBat.VBatWord)
//			ALARM_UV_VBAT_Count = ALARM_UV_VBAT_Count + 1;
//		else
//			ALARM_UV_VBAT_Count = 0;
//	}
//	if(ALARM_UV_VBAT_Count >= ALARM_VBAT_UV_DelayTime2S)
//	{
////            BatteryRealData._VSTATE.bit.wBVUV=1;      //fenglong 20190606
//		ALARM_PT_Status.Bits.wBVUV=1;               //fenglong 20190606
//	}
//	if( (ALARM_VBAT_UV_Release <= BatteryData.VBat.VBatWord) && (ALARM_PT_Status.Bits.wBVUV == 1) )
//	{
//		ALARM_PT_Status.Bits.wBVUV=0;           //fenglong 20190606
//		ALARM_UV_VBAT_Count = 0;                //fenglong 20190531
//	}



	if(FW_PF_Status.Word != 0)
	{
		ALARM_PT_Status.Bits.AL_GLINT=1;
	}
	else if(
			  (FW_PT_Status.Bits.PT_UTSCP == 1)
//			||(FW_PT_Status.Bits.PT_OTSCP == 1)			//Ӧ�ͻ�Ҫ��ȡ�� fenglong 20190708
//			||(FW_PT_Status.Bits.PT_OTSDP == 1)			//Ӧ�ͻ�Ҫ��ȡ�� fenglong 20190714
			||(FW_PT_Status.Bits.PT_OCP == 1)
			||(FW_PT_Status.Bits.PT_ODP == 1)
			||(HW_PT_Status.Bits.HW_SCD ==1)		//��·
			||(HW_PT_Status.Bits.HW_OCD == 1)		//�ŵ��������
//			||(FW_PT_Status.Bits.PT_OTCP == 1)		//fenglong 20190710  V1.83
			||(FW_PT_Status.Bits.PT_UTCP == 1)
			||(FW_PT_Status.Bits.PT_OTDP == 1)
			||(FW_PT_Status.Bits.PT_UTDP == 1)
			||(ALARM_PT_Status.Bits.HW_OTM == 1)
			)
	{
		ALARM_PT_Status.Bits.AL_GLINT=1;
	}
	else
	{
		ALARM_PT_Status.Bits.AL_GLINT=0;
	}

	if( (FW_PT_Status.Bits.PT_OTSDP == 1) && (SystemMode == SystemDischargeMode) )
	{
		Temp_DSG_Start_Check();
	}

	if( SystemMode == SystemChargeMode )
	{
		if( (FW_PT_Status.Bits.PT_UTSCP == 1) || (FW_PT_Status.Bits.PT_OTSCP == 1) )
		{
			Temp_CHG_Start_Check();
		}
	}

//	if( SystemMode != SystemChargeMode )
//	{
//		FW_PT_Status.Bits.PT_UTSCP=0;
////		FW_PT_Status.Bits.PT_OTSCP=0;
//	}

}




