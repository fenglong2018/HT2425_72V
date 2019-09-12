/*
 * sleep.c
 *
 *  Created on: 2019年5月13日
 *      Author: Administrator
 */


//#include "sleep.h"
#include "system.h"



UINT8 SleepCount=0;
UINT8 SysRunCount=0;
UINT8 SysSHIPCount=0;
//extern UINT16 BatteryCapacityRefreshCount;
extern union HW_PT HW_PT_Status;
extern union FW_PT FW_PT_Status;

extern uchar RS485_MODE;
extern UINT16 his_data[FLASH_DATA_NUM];

extern UINT8 SystemMode;

extern RegisterGroup   BQ76930Data;
extern _BatteryData    BatteryData;
//extern _FlashData      BQ76930ProtectData;
//extern union C1 Battery;
//extern union _DataSource     BQ76930ProtectDataSource;
//extern union FW_PF FW_PF_Status;
//extern union HW_PT HW_PT_Status;
//extern union FW_PT FW_PT_Status;

//extern UINT8   BatteryCapacity;
extern union _SW SW_Status;
//extern unsigned char  power_first_flag;
UINT8 Sleep_delay =0;

/*
 *          休眠机制
 *   系统自动判断进入到休眠模式，在休眠模式下，会关闭充放电
 *
 *
 */

void Clr_SCD_OCD()
{
	UINT8 Buff_0, Buff_1;

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

void sleep_lmp3()
{
	if( DC_OUT && KEY_UP && ALERT_DIS && RS485_IDLE && (Sleep_delay > TA0_1s))
	{

		IO_SLEEP();
		SW_Status.bits.SW_Action = 0;		//fenglong 20190703
		WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

		if( DC_OUT && KEY_UP && ALERT_DIS && RS485_IDLE && (Sleep_delay > TA0_1s))
		{
//              	_BIS_SR(LPM0_bits + GIE);         //FENGLONG 20190628
			_BIS_SR(LPM3_bits + GIE); 			//FENGLONG 20190628
			_nop();
		}
		WDTCTL = WDT_ARST_1000;             //AID 20190717
		IO_WAKEUP();

	}

	WDTCTL = WDT_ARST_1000;
	Sleep_delay =0;
	SleepCount =0;
}

void SleepMode(void)
{

//    uchar temp,temp1,i;

    if(SystemMode==SystemSleepMode)
    {


    	Clr_SCD_OCD();

		LED_OFF();

//		if( (DC_OUT) && (KEY_UP) && (RS485_IDLE) && (ALERT_DIS))
//		{
//	        temp=0;		//清楚CC_EN, CHG_ON, DSG_ON
//	    	if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
//	        {
//	            BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
////	            LED_2_High;
//	        }
//
//	    	I2C_Load_BQ76930(&(BQ76930Data.SysCtrl2.SysCtrl2Byte),SYS_CTRL2,1);
//
//	    	// 确认CC_EN清除
//
//			while(BQ76930Data.SysCtrl2.SysCtrl2Bit.CC_EN)
//			{
//				temp=0;
//				if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
//				{
//					BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
//				}
//				I2C_Load_BQ76930(&(BQ76930Data.SysCtrl2.SysCtrl2Byte),SYS_CTRL2,1);
//				Sleep_delay=0;
//				WDTCTL = WDT_ARST_1000;             //AID 20190717
//			}
//		}
//		else
//		{
//			Sleep_delay=0;
//		}

//        if( (Sleep_delay > TA0_1s)  && (DC_OUT) && (KEY_UP) && (RS485_IDLE) )
//        {

//            for(i=30;i>0;i--)
//            {
//            	if( (DC_IN) || (KEY_DOWN) || (RS485_BUSY))
//            	{
//            		Sleep_delay = 0 ;
//            		i=1;
//            	}
//            	// 关 CC_EN, CHG_ON,  DSG_ON
//                temp=0;
//            	if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
//                {
//                    BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
//                }
//
//            	if( I2C_Load_BQ76930(&(BQ76930Data.SysStatus.StatusByte), SYS_STAT, 1) == 0 )
//            	{
//                	//清除DEVICE_XREADY，，OVRD_ALERT，，
////                    if ( (BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY == 1) && (Sleep_delay >= TA0_5s) )
//            		if(BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY == 1)
//                    {
////            			LED_1_High;
//                    	temp1 = BQ76930Data.SysStatus.StatusByte;
//                        BQ76930Data.SysStatus.StatusByte = 0x00;
//                        BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY = 1;
//
//                        temp = I2C_Master_TransmitByte(BQ76930_SysState_Addr,
//                                                         BQ76930Data.SysStatus.StatusByte);
//                        if (temp == 0)
//                        {
//                            BQ76930Data.SysStatus.StatusByte = temp1;
//                            BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY = 0;
//                        }
//                    }
//                    if (BQ76930Data.SysStatus.StatusBit.OVRD_ALERT == 1)
//                    {
////                    	LED_2_High;
//                    	temp1 = BQ76930Data.SysStatus.StatusByte;
//                        BQ76930Data.SysStatus.StatusByte = 0x00;
//                        BQ76930Data.SysStatus.StatusBit.OVRD_ALERT = 1;
//
//                        temp = I2C_Master_TransmitByte(BQ76930_SysState_Addr,
//                                                         BQ76930Data.SysStatus.StatusByte);
//                        if (temp == 0)
//                        {
//                            BQ76930Data.SysStatus.StatusByte = temp1;
//                            BQ76930Data.SysStatus.StatusBit.OVRD_ALERT = 0;
//                        }
//                    }
//                	if(BQ76930Data.SysStatus.StatusBit.CC_READY == 1)
//                	{
//                		temp = 0x80;
//                		I2C_Master_TransmitByte(BQ76930_SysState_Addr, temp);
//                	}
//            	}
//            }

		sleep_lmp3();

                //恢复BQ配置
//			temp = BQ76930Data.SysCtrl2.SysCtrl2Byte | 0x40 ;       //ENABLE CC_EN
//			if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
//			{
//				BQ76930Data.SysCtrl2.SysCtrl2Byte |= 0x40 ;
//			}

//        }

    }
    else
    {
        Sleep_delay =0;
    }

}


void SleepCheck(void)
{
    UINT16 temp_cur=0;
    if((BatteryData.CC.CCWord >= 0x8000)&&(SystemMode == SystemDischargeMode))
    {
        temp_cur= 0- BatteryData.CC.CCWord ;

        if( temp_cur >= SleepCurrent )
        {
            SleepCount=0;
        }
    }

    if((BatteryData.CC.CCWord >= 0x8000)&&(SystemMode == SystemSleepMode))
    {
        temp_cur= 0- BatteryData.CC.CCWord ;

        if( temp_cur >= SleepCurrent )
        {
            SleepCount=0;
            SystemMode = SystemDischargeMode;
        }
    }

    if( (SystemMode == SystemChargeMode )
    	||(SystemMode == SystemBootUp)
		||(SystemMode == SystemBootUpWait)
    		)
    {
        SleepCount=0;
    }

    if((SleepCount > TA0_300s) && (SystemMode != SystemSleepMode ))
    {
        SystemMode = SystemSleepMode;
        //SysRunCount=0;
        SleepCount=0;
        RS485_MODE = 0 ;            //fenglong 2019603

                                    //fenglong 20190617  准备增加保存FLASH
    }
//    if(SystemMode == SystemSleepMode)
//    {
//        SleepCount=0;
//    }
}



void ShutDownMode()
{
	uchar temp,temp1,i;
	if(SystemMode == SystemShutDown)
	{
		Clr_SCD_OCD();
		LED_OFF();

		if( (DC_OUT) && (KEY_UP) && (RS485_IDLE) && (ALERT_DIS))
		{
			temp=0;		//清楚CC_EN, CHG_ON, DSG_ON
			if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
			{
				BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
//	            LED_2_High;
			}

			I2C_Load_BQ76930(&(BQ76930Data.SysCtrl2.SysCtrl2Byte),SYS_CTRL2,1);

			// 确认CC_EN清除

			while(BQ76930Data.SysCtrl2.SysCtrl2Bit.CC_EN)
			{
				temp=0;
				if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
				{
					BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
				}
				I2C_Load_BQ76930(&(BQ76930Data.SysCtrl2.SysCtrl2Byte),SYS_CTRL2,1);
				Sleep_delay=0;
				WDTCTL = WDT_ARST_1000;             //AID 20190717
			}
		}
		else
		{
			Sleep_delay=0;
		}

		if( (Sleep_delay > TA0_1s)  && (DC_OUT) && (KEY_UP) && (RS485_IDLE) )
		{

			for(i=30;i>0;i--)
			{
				if( (DC_IN) || (KEY_DOWN) || (RS485_BUSY))
				{
					Sleep_delay = 0 ;
					i=1;
				}
				// 关 CC_EN, CHG_ON,  DSG_ON
				temp=0;
				if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
				{
					BQ76930Data.SysCtrl2.SysCtrl2Byte &= temp ;
				}

				if( I2C_Load_BQ76930(&(BQ76930Data.SysStatus.StatusByte), SYS_STAT, 1) == 0 )
				{
					//清除DEVICE_XREADY，，OVRD_ALERT，，
//                    if ( (BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY == 1) && (Sleep_delay >= TA0_5s) )
					if(BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY == 1)
					{
//            			LED_1_High;
						temp1 = BQ76930Data.SysStatus.StatusByte;
						BQ76930Data.SysStatus.StatusByte = 0x00;
						BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY = 1;

						temp = I2C_Master_TransmitByte(BQ76930_SysState_Addr,
														 BQ76930Data.SysStatus.StatusByte);
						if (temp == 0)
						{
							BQ76930Data.SysStatus.StatusByte = temp1;
							BQ76930Data.SysStatus.StatusBit.DEVICE_XREADY = 0;
						}
					}
					if (BQ76930Data.SysStatus.StatusBit.OVRD_ALERT == 1)
					{
//                    	LED_2_High;
						temp1 = BQ76930Data.SysStatus.StatusByte;
						BQ76930Data.SysStatus.StatusByte = 0x00;
						BQ76930Data.SysStatus.StatusBit.OVRD_ALERT = 1;

						temp = I2C_Master_TransmitByte(BQ76930_SysState_Addr,
														 BQ76930Data.SysStatus.StatusByte);
						if (temp == 0)
						{
							BQ76930Data.SysStatus.StatusByte = temp1;
							BQ76930Data.SysStatus.StatusBit.OVRD_ALERT = 0;
						}
					}
					if(BQ76930Data.SysStatus.StatusBit.CC_READY == 1)
					{
						temp = 0x80;
						I2C_Master_TransmitByte(BQ76930_SysState_Addr, temp);
					}
				}
			}
		}

		sleep_lmp3();

            //恢复BQ配置
		temp = BQ76930Data.SysCtrl2.SysCtrl2Byte | 0x40 ;       //ENABLE CC_EN
		if( I2C_Master_TransmitByte(BQ76930_SYS_CTRL2_Addr, temp) == 0)
		{
			BQ76930Data.SysCtrl2.SysCtrl2Byte |= 0x40 ;
		}

	}
}



void ShipMode(void)
{
    UINT16 crc16;
    UINT8 i;
	if((HW_PT_Status.Bits.HW_UV ==1)||(SystemMode == SystemShipMode) || (FW_PT_Status.Bits.PT_UV == 1))
	{
	    SysSHIPCount++;

	    if(SysSHIPCount>4)		//fenglong 20190625
	    {
            if((HW_PT_Status.Bits.HW_UV == 1)||(FW_PT_Status.Bits.PT_UV == 1))
            {
                his_data[HIS_DSG_UVP_ADD] = 1;              // 20190521  AID
            }
            else
            {
                his_data[HIS_DSG_UVP_ADD] = 0;              // 20190521  AID
            }

//fenglong 20190730
            his_data[FLASH_CYCLES_ADDR] = sys_cap.val.bat_cycle_cnt;
            his_data[CHG_CAP_CNT] = sys_cap.val.chg_cap_cnt;

//fenglong 20190705
			crc16 =0;
			for(i=0;i<FLASH_DATA_NUM-1;i++)             //fenglong 20190615
			{
				crc16 += his_data[i];
			}
			his_data[HIS_CRC16] = crc16;
			__bic_SR_register(  GIE);       // Enter LPM3 w/ interrupt
			Falsh_Write_Arr((unsigned int *)Segment_C,his_data,FLASH_DATA_NUM);         //fenglong 20190615
			Falsh_Write_Arr((unsigned int *)Segment_D,his_data,FLASH_DATA_NUM);         //fenglong 20190615
			__bis_SR_register(  GIE);       // Enter LPM3 w/ interrupt



            while(1)
            {
//                _delay_cycles(250);			//fenglong 100806 需要屏蔽
//                _delay_cycles(250);			//fenglong 100806 需要屏蔽
                BQ_Shutdown();
            }
	    }
	}
	else
	{
	    SysSHIPCount =0;
	}

}


void IO_SLEEP(void)
{
//    P1DIR &= ~BIT3;     // key     //0输入，1输出			fenglong 190703
//    P1DIR &= ~BIT4;     //DC
//    P1DIR &= ~BIT5;     //BQ
    P2DIR &= ~BIT3;     //485

//    P1IES &= ~BIT3;     // key    	//0  低电平到高电平			fenglong 190703
//    P1IES |= BIT4;     //DC			//1 高电平到低电平
 //   P1IES &= ~BIT5;     //BQ		//0  低电平到高电平
    P2IES |= BIT3;     //485		//1 高电平到低电平

//    P1IFG &= ~BIT3;     //清除IO口中断标志     key			fenglong 190703
//    P1IFG &= ~BIT4;     //清除IO口中断标志     DC
//    P1IFG &= ~BIT5;     //清除IO口中断标志     BQ
    P2IFG &= ~BIT3;     //清除IO口中断标志     485

//    P1IE |= BIT3;     // key    	//0  低电平到高电平			fenglong 190703
//    P1IE |= BIT4;     //DC			//1 高电平到低电平
//    P1IE |= BIT5;     //BQ			//0  低电平到高电平
    P2IE |= BIT3;     //485			//1 高电平到低电平

	P1DIR|=BIT0;

    P1OUT|=BIT0;
    RT_ON_HIGH;

    CP_Low;

    P1DIR |= BIT1 + BIT2;	// + BIT6 + BIT7;
    P1OUT |= BIT1 + BIT2;	// + BIT6 + BIT7;
    RE_485_High;
    DE_485_Low;
    IFG2 &= ~(UCA0RXIFG);
    IE2 &= ~UCA0RXIE;                          // Disable USCI_A0 RX interrupt

}


void IO_WAKEUP(void)
{

	UART_Initial();


	CP_High;

//    P1IFG &= ~BIT3;     //清除IO口中断标志     key
//    P1IFG &= ~BIT4;     //清除IO口中断标志     DC
//    P1IFG &= ~BIT5;     //清除IO口中断标志     BQ
    P2IFG &= ~BIT3;     //清除IO口中断标志     485

//    P1DIR &= ~BIT3;     // key     //0输入，1输出			fenglong 190703
//    P1DIR &= ~BIT4;     //DC
//    P1DIR &= ~BIT5;     //BQ
    P2DIR &= ~BIT3;     //485

//    P1IE |= BIT3;     // key    	//0  低电平到高电平			fenglong 190703
//    P1IE &= ~BIT4;     //DC			//1 高电平到低电平
//    P1IE &= ~BIT5;     //BQ			//0  低电平到高电平
    P2IE &= ~BIT3;     //485			//1 高电平到低电平

}


void sleep_lmp0()
{
	if(SystemMode == SystemDischargeMode)
	{
		if(SysRunCount > 1)
		{
//			LED_OFF();
			if(DC_OUT && KEY_UP  && RS485_IDLE)
			{
//				LED_1_High;
				WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
				_BIS_SR(LPM0_bits + GIE); 			//FENGLONG 20190628
				_nop();
				WDTCTL = WDT_ARST_1000;
			}
			SysRunCount=0;
		}
		else
		{
			SysRunCount++;
		}
	}
	else
	{
		SysRunCount=0;
	}
}




