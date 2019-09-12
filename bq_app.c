







#include "bq_app.h"
#include "System.h"

extern RegisterGroup BQ76930Data;
extern union HW_PT HW_PT_Status;
extern union FW_PT FW_PT_Status;
extern _BatteryData BatteryData;

void GetADCGainOffset()
{
    I2C_Load_BQ76930(&(BQ76930Data.ADCGain1.ADCGain1Byte), ADCGAIN1,1);
    I2C_Load_BQ76930(&(BQ76930Data.ADCGain2.ADCGain2Byte), ADCGAIN2,1);
    I2C_Load_BQ76930(&(BQ76930Data.ADCOffset), ADCOFFSET,1);
}




void LoadFlashData(void)
{

    if(I2C_Load_BQ76930(&(BQ76930Data.SysCtrl1.SysCtrl1Byte),SYS_CTRL1,2) ==0)
    {
        if(((BQ76930Data.SysCtrl1.SysCtrl1Byte&0x18) != 0x18)||((BQ76930Data.SysCtrl2.SysCtrl2Byte&0x40) != 0x40)||(BQ76930Data.CCCfg!= 0x19))
        {
        	BQ_Init();
        }
    }
}

void BQ_Shutdown(void)
{
    BQ76930Data.SysCtrl1.SysCtrl1Bit.SHUT_A=0;
    BQ76930Data.SysCtrl1.SysCtrl1Bit.SHUT_B=1;
    I2C_Master_TransmitByte(SYS_CTRL1, BQ76930Data.SysCtrl1.SysCtrl1Byte);
    _delay_cycles(250);
    BQ76930Data.SysCtrl1.SysCtrl1Bit.SHUT_A=1;
    BQ76930Data.SysCtrl1.SysCtrl1Bit.SHUT_B=0;
    I2C_Master_TransmitByte(SYS_CTRL1, BQ76930Data.SysCtrl1.SysCtrl1Byte);
}


void Clr_bq_alert(void)
{
    uchar temp;
    uint iTemp;


	if( I2C_Load_BQ76930(&(BQ76930Data.SysStatus.StatusByte), SYS_STAT, 1) == 0 )
	{
		if(BQ76930Data.SysStatus.StatusBit.CC_READY == 1)
		{
//			result = I2C_Load_BQ76930(&(BQ76930Data.CC.CCByte.CC_HI), CC_HI_BYTE, 2);
			if(I2C_Load_BQ76930(&(BQ76930Data.CC.CCByte.CC_HI), CC_HI_BYTE, 2) ==0)
			{
				iTemp = U16_SWAP(BQ76930Data.CC.CCWord);

				if(iTemp >0x8000)
				{
					iTemp = 0-iTemp;
					BatteryData.CC.CCWord = (long)iTemp*844/250;
					BatteryData.CC.CCWord = 0-BatteryData.CC.CCWord;
				}
				else
				{
					BatteryData.CC.CCWord = (long)iTemp*844/250;
				}
			}

			Soc_AddSub();
			temp = 0x80;
			I2C_Master_TransmitByte(BQ76930_SysState_Addr, temp);
		}

		if(BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY == 1)
		{
//            			LED_1_High;
//			temp1 = BQ76930Data.SysStatus.StatusByte;
//			BQ76930Data.SysStatus.StatusByte = 0x00;
//			BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY = 1;
			temp = 0x20;
			I2C_Master_TransmitByte(BQ76930_SysState_Addr, temp);
		}
		if (BQ76930Data.SysStatus.StatusBit.OVRD_ALERT == 1)
		{
//                    	LED_2_High;
//			temp1 = BQ76930Data.SysStatus.StatusByte;
//			BQ76930Data.SysStatus.StatusByte = 0x00;
//			BQ76930Data.SysStatus.StatusBit.OVRD_ALERT = 1;

			temp=0x10;
			I2C_Master_TransmitByte(BQ76930_SysState_Addr, temp);
		}
	}

}


void clr_scd_ocd(void)
{
	uchar Buff_0, Buff_1;
    FW_PT_Status.Bits.PT_OCP =0;
	FW_PT_Status.Bits.PT_ODP =0;
	while (((HW_PT_Status.Bits.HW_OCD == 1) || (HW_PT_Status.Bits.HW_SCD == 1)))
	{
		Buff_1 = HW_PT_Status.Byte;
		BQ76930Data.SysStatus.StatusByte =0;
		//HW_PT_Status.Byte = 0x00;
		//HW_PT_Status.Bits.HW_OCD = 1;           //放电过流2级保护
		//HW_PT_Status.Bits.HW_SCD = 1;           //short current detection  短路监测电流
		if(HW_PT_Status.Bits.HW_OCD == 1)
		{
		  BQ76930Data.SysStatus.StatusBit.OCD = 1;
		}
		if(HW_PT_Status.Bits.HW_SCD == 1)
		{
		  BQ76930Data.SysStatus.StatusBit.SCD = 1;
		}
		Buff_0 = I2C_Master_TransmitByte(BQ76930_SysState_Addr,
									   BQ76930Data.SysStatus.StatusByte);

		if (Buff_0 == 0)
		{
		  HW_PT_Status.Byte = Buff_1;
		  HW_PT_Status.Bits.HW_OCD = 0;       //放电过流2级保护
		  HW_PT_Status.Bits.HW_SCD = 0;       //short current detection  短路监测电流
//			  BQ76930Data.SysStatus.StatusByte =0;
		  BQ76930Data.SysStatus.StatusByte = HW_PT_Status.Byte;
		}
		WDTCTL = WDT_ARST_1000;
	}
}

void read_bq(void)
{
	if(ALERT_ON)
	{

//		LED_4_High;

		Update_BQ_Data();
		His_DataRenew();
		Clr_bq_alert();

//		LED_4_Low;
	}
}










