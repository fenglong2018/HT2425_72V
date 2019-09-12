/*
 * RS_485.c
 *
 *  Created on: 2019骞�4鏈�17鏃�
 *      Author: 20190211012
 */

#include "system.h"


_BatteryRealData    BatteryRealData;
uchar SFET;
uchar CRC_DATA=0;

uchar RS485_MODE=0;
extern RegisterGroup   BQ76930Data;
extern _BatteryData    BatteryData;
//extern _FlashData      BQ76930ProtectData;
//extern union C1 Battery;
extern union FW_PF FW_PF_Status;
extern union HW_PT HW_PT_Status;
extern union FW_PT FW_PT_Status;
extern union SW_ALARM ALARM_PT_Status;
//extern union _LED_CTRL LED_CTRL;
//extern UINT8   BatteryCapacity;
//extern UINT16 SleepCount;
extern UINT8 SystemMode;
extern UINT16 his_data[FLASH_DATA_NUM];
extern UINT16 CellMiniVoltage;
//extern UINT16 CellMAXiVoltage;

//extern uint8_t usartTxBuffer[TX_BUFFER_SIZE];
extern volatile uchar usartRxbuffer[];
//extern uchar MAXVoltage_Num;                    //fenglong 20190620

//extern UINT16 BatteryCapacityRefreshCount;
//extern UINT16 TOFF_DHG_MOS_Count;

//const uchar BATTRYPRODATA1[] =
//{
//    0x01,       0x81,       0x52,       0x00,0xEA,     0x01,       0x07,
//    //addr/ cmd  /  ver82 / len  /  addr /   CellNum
//    0x00,       0x00,0xA0,     0x00,0x6D,     0x0C,0xE4,
////CSGLimitEn/   EngDesign/  Rsense/     Verf 3300
//    02,         0x5A,       0x23,       0x0F,0xA0,     0x00,0x1E,
////B.Mode /      B.THDIS/    B.TLDIS/    B.VStart/   B.VDiff
//    0x10,0x04,  0x0C,0xE4,     0x78,0x1E,     0x60,0xAE,     0x5F,
////W.VCell.H/    W.VCell.L/  W.VBAT.H/   W.VBAT.L/   W.TCell.H
//    0x28,       0x5F,       0x28,       0x69,       0x28,
////W.TCell.L     /W.TENV.H/  W.TENV.L/   W.TFET.H/   W.TFET.L
//    0x06,0x40,     0x0D,0xAC,     0x03,0xE8,     0x03,0x84,
////W.CURR.C/     W.CURR.D/   W.VDIFF.H/  W.VDIFF.L
//    0x10,0x9A,     0x00,0x64,     0x10,0x04,     0xA,0xF0,      0x00,0x64,     0x0C,0xE4,
////OVPVAL        OVPDLY      OVPREL      UVPVAL      UVPDLY      UVPREL
//    0x7B,0x0C,     0x00,0xC8,     0x78,0x1E,     0X57,0xE4,     0x01,0xF4,     0x60,0xAE,
////BOVPVAL       BOVPDLY     BOVPREL     BUVPVAL     BUVPDLY     BUVPREL
//    0x5F,       0x5A,       0x28,       0x2B,
////CTcellHPro    CTcellHRel  CTcellPro   CTcellLRel
//    0x6E,       0x64,       0x14,       0x19,
////DTcellHPro    DTcellHRel  DTcellLPro  DTcellLRel
//    0x6E,       0x64,       0x14,       0x19,
////TenvHPro      TenvHRel    TenvLPro    TenvLRel
//    0x78,       0x6E,       0,          0x0f,
////TfetHPro      TfetHRel    TfetLPro    TfetLRel
//    0x06,0x40,     0x06,0x40,     0x0B,0xB8,     0x00,0x03,
////CC.PRO.VAL  CC.PRO.PDLY  CC.PRO.RDLY  CC.PRO.LOCK
//    0x0D,0xAC,     0x00,0x64,     0x0B,0xB8,     0X00,0x03,
////CD1.PRO.VAL CD1.PRO.PDLY CD1.PRO.RDLY CD1.PRO.LOCK
//    0x11,0x94,     0x00,0x0A,     0x0B,0xB8,     0x00,0x03,
////CD2.PRO.VAL CD2.PRO.PDLY CD2.PRO.RDLY CD2.PRO.LOCK
//    0x01,       0x03,0xE8,     0x00,0x03,
////SHORT.VAL     SHORT.RDLY  SHORT.LOCK
//    0,          0x23,       0x2D   //  0xF2,
////HEAT.EN       HEAT.TSTART HEAT.TEND   CRC
//};

//BatteryRealData.Addr=0x01;
//BatteryRealData.cmd=0x82;
//BatteryRealData.ver=BMS_VER;
//BatteryRealData.Len.val=RD_REAL_NUM;
//
//BatteryRealData.Time_t.BMS_year=19;
//BatteryRealData.Time_t.BMS_month=5;
//BatteryRealData.Time_t.BMS_data=01;
//BatteryRealData.Time_t.BMS_hour=0;
//BatteryRealData.Time_t.BMS_minute=0;
//BatteryRealData.Time_t.BMS_second=0;
//
//BatteryRealData.Time_t.BMS_week=17;

const UINT8 BATTARYREALDATA1[] =
{
     0x01,  0x82,   0x52,   0x00,   0x84,
     0x13,  0x05,   0x01,   0x00,   0x00,   0x00,   0x11
};

const uchar BATTARYREALDATA_CELL_NUM[]={0x07};
const uchar BATTARYREALDATA_TEMPNUM[]={0x02};
const uchar BATTARYREALDATA_BLANK[]={0x00};
//const uchar SCUD={0x53,0x43,0x55,0x44};









void RS485_REC_Task(void)
{
//    int8_t temp;
//    int8_t  crc_h, crc_l;
    uchar i;
    uchar j;

	if (RS_485.RxStatus==USART_RX_OK)
	{
//	    temp = crc8_cal(&usartRxbuffer[1],RS_485.RxCount-4);
//
//	    crc_h=temp & 0xF0 ;
//	    if(crc_h>="a")
//        {
//           crc_h = crc_h + 0x61;
//        }
//        else if(crc_h>9)
//        {
//            crc_h = crc_h + 0x37;
//        }
//        else
//        {
//            crc_h = crc_h + 0x30;
//        }
//
//	    crc_l=temp & 0x0F ;
//	    if(crc_l>="a")
//	    {
//	        crc_l = crc_l + 0x61;
//	    }
//	    else if(crc_l>9)
//        {
//            crc_l = crc_l + 0x37;
//        }
//        else
//        {
//            crc_l = crc_l + 0x30;
//        }


//        if(( crc_h == usartRxbuffer[RS_485.RxCount-3]) && ( crc_l == usartRxbuffer[RS_485.RxCount-2]))       //CRC OK
        if(1)               //因为客户CRC采用手写赋值，不区分大小写，有时用大写，有时用小写，与校验值不符，故只能屏蔽接受CRC判断
        {
            uint16_t cmd;

            cmd = usartRxbuffer[CMD_ADD];
            cmd<<=8;

            cmd = cmd + usartRxbuffer[CMD_ADD+1] ;
            switch (cmd)
             {
//                 case RD_PRO_PARA:

//                     RS485_DE;
//                     CRC_DATA = 0 ;
//
//                     RS485_Char_TX(':');
//
//                     UART_Transmit(BATTRYPRODATA1,sizeof(BATTRYPRODATA1));
//
////                     temp=crc8_cal(BATTRYPRODATA1,sizeof(BATTRYPRODATA1));
////
////                     UART_Transmit((uint)&temp,1);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<60;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;

                 case RD_REAL_PARA:

                     RS485_DE;
                     CRC_DATA = 0 ;

                     RS485_Char_TX(':');

                     UART_Transmit((uchar *)BATTARYREALDATA1,sizeof(BATTARYREALDATA1));      //fenglong 20190617

//                     UART_Transmit((uint)&BatteryRealData,5);                     //fenglong 20190617
//
//                     UART_Transmit((uint)&BatteryRealData+6,7);                   //fenglong 20190617

//                     UART_Transmit((uint)&BatteryRealData+14,3);Vcell_num         //fenglong 20190617
                     UART_Transmit((uchar *)&BatteryRealData.BMS_Vbat,2);              //fenglong 20190617
                     UART_Transmit((uchar *)&BATTARYREALDATA_CELL_NUM[0],1);
                     UART_Transmit((uchar *)&BatteryRealData.Vcell,14);
                     UART_Transmit((uchar *)&BatteryRealData.Curr,4);
                     UART_Transmit((uchar *)BATTARYREALDATA_TEMPNUM,1);
                     UART_Transmit((uchar *)&BatteryRealData.Temp,2);
                     UART_Transmit((uchar *)&BatteryRealData._VSTATE,8);
                     UART_Transmit((uchar *)&BatteryRealData._FETSTATE,1);

                     UART_Transmit((uchar *)BATTARYREALDATA_BLANK,1);
                     UART_Transmit((uchar *)&BatteryRealData.NUM_VOV,1);
                     UART_Transmit((uchar *)BATTARYREALDATA_BLANK,1);
                     UART_Transmit((uchar *)&BatteryRealData.NUM_VUV,1);
                     UART_Transmit((uchar *)BATTARYREALDATA_BLANK,1);
                     UART_Transmit((uchar *)&BatteryRealData.NUM_WARN_VHIGH,1);
                     UART_Transmit((uchar *)BATTARYREALDATA_BLANK,1);
                     UART_Transmit((uchar *)&BatteryRealData.NUM_WARN_VLOW,1);

                     UART_Transmit((uchar *)BATTARYREALDATA_BLANK,1);
                     UART_Transmit((uchar *)&BatteryRealData.BlanceState,1);               //fenglong 20190620

                     UART_Transmit((uchar *)&BatteryRealData.DchgNum,4);
                     UART_Transmit((uchar *)&BatteryRealData.SOC,1);
                     UART_Transmit((uchar *)&BatteryRealData.CapNow,4);

                     BatteryRealData.CRC = CRC_DATA^0xFF;
                     UART_Transmit((uchar *)&BatteryRealData.CRC,1);

                     RS485_Char_TX('~');
                     CRC_DATA = 0 ;
                     for(j=0;j<50;j++)
                     {
                         for(i=0;i<=RX_BUFFER_SIZE;i++)
                         {
                             usartRxbuffer[i] = 0x00;
                         }

                     }

                     RS_485.RxCount=0;
                     RS_485.RxStatus = USART_RX_IDLE;
                     RS485_RE;

                     break;
//                 case WR_FET_PARA:

//                     CRC_DATA = 0 ;
//
//                     switch(usartRxbuffer[12])
//                     {
//                         case   0x30:
//                             if(SystemMode == SystemDischargeMode)
//                             {
//                                 SystemMode = SystemSleepMode;
//                             }
//                             LED_CTRL.Word =0;
//                             LED_OFF();
//                             close_timer1();
//                             //SFET=0X00;
//                             break;
//                         case   0x31:
//
//                             if(SystemMode == SystemSleepMode)              //fenglong 20190602
//                             {
////                                 if(Temp_DSG_Start_Check()==0)                   //fenglong 20190611
////                                 {
////                                     SystemMode = SystemDischargeMode;
////                                     RS485_MODE=1;
//////                                     ALARM_PT_Status.Bits.AL_GLINT=0;                    //fenglong 20190702 报警闪烁
////                                 }
////                                 else
////                                 {
////                                     SystemMode = SystemSleepMode;
////                                 }
//                            	 Temp_DSG_Start_Check();
//                                 SystemMode = SystemDischargeMode;        //fenglong 20190611
//                                 RS485_MODE=1;                              //fenglong 20190611
//                                 SleepCount=0;
//                             }
//                             LED_CTRL.Word =0;
//                             LED_OFF();
//                             close_timer1();
//
////                             BatteryCapacityRefreshCount = TA0_10s - TA0_2s ;		//fenglong 20190714
//                             //SFET=0X01;
//                             break;
//                         case   0x32:
//                             SFET=0X02;
//                             break;
//                         default:
//                             break;
//                     }
//
//                     RS485_DE;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x8A;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x10;
//                     usartRxbuffer[5] = 0x06;
////                     usartRxbuffer[6] = crc8_cal(&usartRxbuffer, 6);
//
//
//                     RS485_Char_TX(':');
//
//                     UART_Transmit(&usartRxbuffer,6);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;

//                 case RD_CAP_PARA:

//                     RS485_DE;
//                     CRC_DATA = 0 ;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x90;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x1E;
//
//                     usartRxbuffer[5] = 0x00;
//                     usartRxbuffer[6] = 0x00;
//
//                     usartRxbuffer[7] = BatteryRealData.CapNow.val >> 8;
//                     usartRxbuffer[8] = BatteryRealData.CapNow.val & 0x00ff;
//
//                     usartRxbuffer[9] = BatteryRealData.CapFull.val >> 8;
//                     usartRxbuffer[10] = BatteryRealData.CapFull.val & 0x00ff;
//
//                     usartRxbuffer[11] = BatteryRealData.CapFull.val >> 8;
//                     usartRxbuffer[12] = BatteryRealData.CapFull.val & 0x00ff;
//
////                     usartRxbuffer[13] =CRC_DATA;
//
//                     RS485_Char_TX(':');
//                     UART_Transmit(&usartRxbuffer,13);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;

//                 case RD_SN_PARA:

//                     RS485_DE;
//                     CRC_DATA = 0 ;
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x92;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x16;
//                     usartRxbuffer[5] = 0x06;
//
//
//                     usartRxbuffer[6] = 0x12 ;
//                     usartRxbuffer[7] = 0x34 ;
//                     usartRxbuffer[8] = 0x56 ;
//
////                     usartRxbuffer[9] = crc8_cal(&usartRxbuffer, 9);
//
//                     RS485_Char_TX(':');
//                     UART_Transmit(&usartRxbuffer,9);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;
//                 case RD_SOC_PARA:

//                     CRC_DATA = 0 ;
//                     for(i=0;i<=RX_BUFFER_SIZE;i++)
//                     {
//                         usartRxbuffer[i] = 0x00;
//                     }
//
//                     RS485_DE;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x93;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x10;
//                     usartRxbuffer[5] = BatteryRealData.SOC;
////                     usartRxbuffer[6] =crc8_cal(&usartRxbuffer, 6);
//
//
//                     RS485_Char_TX(':');
//                     UART_Transmit(&usartRxbuffer,6);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;

                 case WR_SHIP_PARA:

                     CRC_DATA = 0 ;
                     for(i=0;i<=RX_BUFFER_SIZE;i++)
                     {
                         usartRxbuffer[i] = 0x00;
                     }

                     RS485_DE;

                     usartRxbuffer[0] = 0x01;
                     usartRxbuffer[1] = 0x11;
                     usartRxbuffer[2] = BMS_VER;
                     usartRxbuffer[3] = 0x00;
                     usartRxbuffer[4] = 0x0E;
//                     usartRxbuffer[5] =crc8_cal(&usartRxbuffer, 6);


                     RS485_Char_TX(':');
                     UART_Transmit((uchar *)&usartRxbuffer,5);

                     BatteryRealData.CRC = CRC_DATA^0xFF;
                     UART_Transmit((uchar *)&BatteryRealData.CRC,1);

                     RS485_Char_TX('~');
                     CRC_DATA = 0 ;



                     for(j=0;j<50;j++)
                     {
                         for(i=0;i<=RX_BUFFER_SIZE;i++)
                         {
                             usartRxbuffer[i] = 0x00;
                         }

                     }

                     RS_485.RxCount=0;
                     RS_485.RxStatus = USART_RX_IDLE;
                     RS485_RE;

                     SystemMode = SystemShipMode;

                     break;

//                 case WR_PRODUCTION_DATA:                   //fenglong 20190617

//                     CRC_DATA = 0 ;
//
//                     his_data[PRODUCTION_DDMM] = ASC2HEX(usartRxbuffer[11],usartRxbuffer[12]);
//                     his_data[PRODUCTION_DDMM] <<= 8;
//
//                     his_data[PRODUCTION_DDMM] = his_data[PRODUCTION_DDMM] + ASC2HEX(usartRxbuffer[13],usartRxbuffer[14]);
//
//                     his_data[PRODUCTION_YY] = ASC2HEX(usartRxbuffer[15],usartRxbuffer[16]);
//                     his_data[PRODUCTION_YY] <<= 8;
//
//                     RS485_DE;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x8A;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x14;
//                     usartRxbuffer[5] = his_data[PRODUCTION_DDMM]>>8;
//                     usartRxbuffer[6] = his_data[PRODUCTION_DDMM] & 0x00ff;
//                     usartRxbuffer[7] = his_data[PRODUCTION_YY]>>8;
//
//                     RS485_Char_TX(':');
//
//                     UART_Transmit(&usartRxbuffer,8);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;

//                 case RD_MANUFACTURER:                      //fenglong 20190617


//                     CRC_DATA = 0 ;
//                     for(i=0;i<=RX_BUFFER_SIZE;i++)
//                     {
//                         usartRxbuffer[i] = 0x00;
//                     }
//
//                     RS485_DE;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x15;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x16;
//                     usartRxbuffer[5] = 0x53;
//                     usartRxbuffer[6] = 0x43;
//                     usartRxbuffer[7] = 0x55;
//                     usartRxbuffer[8] = 0x44;
//
//                     RS485_Char_TX(':');
//                     UART_Transmit(&usartRxbuffer,9);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//
//                     break;


//                 case RD_PRODUCTION_DATA:                   //fenglong 20190617

//                     CRC_DATA = 0 ;
//                     for(i=0;i<=RX_BUFFER_SIZE;i++)
//                     {
//                         usartRxbuffer[i] = 0x00;
//                     }
//
//                     RS485_DE;
//
//                     usartRxbuffer[0] = 0x01;
//                     usartRxbuffer[1] = 0x16;
//                     usartRxbuffer[2] = BMS_VER;
//                     usartRxbuffer[3] = 0x00;
//                     usartRxbuffer[4] = 0x14;
//
//                     usartRxbuffer[5] = (his_data[PRODUCTION_DDMM] & 0xFF00)>>8;
//                     usartRxbuffer[6] = (his_data[PRODUCTION_DDMM] & 0x00FF);
//                     usartRxbuffer[7] = (his_data[PRODUCTION_YY] & 0xFF00)>>8;
//
//                     RS485_Char_TX(':');
//                     UART_Transmit(&usartRxbuffer,8);
//
//                     BatteryRealData.CRC = CRC_DATA^0xFF;
//                     UART_Transmit((uint)&BatteryRealData.CRC,1);
//
//                     RS485_Char_TX('~');
//                     CRC_DATA = 0 ;
//
//                     for(j=0;j<50;j++)
//                     {
//                         for(i=0;i<=RX_BUFFER_SIZE;i++)
//                         {
//                             usartRxbuffer[i] = 0x00;
//                         }
//
//                     }

//                     RS_485.RxCount=0;
//                     RS_485.RxStatus = USART_RX_IDLE;
//                     RS485_RE;
//                     break;

                 case CLR_PF_DATA:                   //fenglong 20190706

                     CRC_DATA = 0 ;
                     for(i=0;i<=RX_BUFFER_SIZE;i++)
                     {
                         usartRxbuffer[i] = 0x00;
                     }

                     RS485_DE;

                     CLR_PF();

                     usartRxbuffer[0] = 0x01;
                     usartRxbuffer[1] = 0x28;
                     usartRxbuffer[2] = 0x8A;
                     usartRxbuffer[3] = BMS_VER;
                     usartRxbuffer[4] = 0x00;
                     usartRxbuffer[5] = 0x10;

                     RS485_Char_TX(':');
                     UART_Transmit((uchar *)&usartRxbuffer[0],6);

                     BatteryRealData.CRC = CRC_DATA^0xFF;
                     UART_Transmit((uchar *)&BatteryRealData.CRC,1);

                     RS485_Char_TX('~');
                     CRC_DATA = 0 ;

                     for(j=0;j<50;j++)
                     {
                         for(i=0;i<=RX_BUFFER_SIZE;i++)
                         {
                             usartRxbuffer[i] = 0x00;
                         }
                     }

                     RS_485.RxCount=0;
                     RS_485.RxStatus = USART_RX_IDLE;
                     RS485_RE;
                     break;

                 default:
                     break;

             }
        }

	}
}



//uint8_t crc8_cal(uint8_t *prt, uint8_t len)
//{
//    int16_t crc16=0, i;
//    int8_t  crc8=0;
//
//    for(i=1; i<=len; i++)
//    {
//        crc16+=*(prt);
//        prt++;
//    }
//    crc8=crc16 & 0xFF;
//    crc8^=0xFF;
//    return crc8;
//}

void RS485_Char_TX(int8_t data)
{

    while (!(IFG2&UCA0TXIFG));        // USCI_A0 TX buffer ready? 缂佹稑顦欢鐑糥 buffer濞戞捁娅ｉ埞鏍晬鐏炶棄绲洪梺顐＄婢х姵绋夐敓鐣岋拷瑙勪亢椤╋箑螞閿熻姤顨ュ畝瀣闁告熬绠戦崹顖滐拷纭咁潐濡叉宕橀懠顒傚磹
    UCA0TXBUF = data ;
 //   IFG2 &=~UCA0RXIFG;
    while (!(IFG2&UCA0TXIFG));        // USCI_A0 TX buffer ready?
}





void BatteryRealData_Refresh(void)
{
    UINT16 temp16;

//    BatteryRealData.Addr=0x01;                //fenglong 20190617
//    BatteryRealData.cmd=0x82;
//    BatteryRealData.ver=BMS_VER;
//    BatteryRealData.Len.val=RD_REAL_NUM;
//
//    BatteryRealData.Time_t.BMS_year=19;
//    BatteryRealData.Time_t.BMS_month=5;
//    BatteryRealData.Time_t.BMS_data=01;
//    BatteryRealData.Time_t.BMS_hour=0;
//    BatteryRealData.Time_t.BMS_minute=0;
//    BatteryRealData.Time_t.BMS_second=0;
//
//    BatteryRealData.Time_t.BMS_week=17;



    BatteryRealData.BMS_Vbat.val=U16_SWAP(BatteryData.VBat.VBatWord>>1);

//    BatteryRealData.Vcell_num=CELL_NUM;

    BatteryRealData.Vcell[0].val=U16_SWAP(BatteryData.VCell1.VCell1Word);
    BatteryRealData.Vcell[1].val=U16_SWAP(BatteryData.VCell2.VCell2Word);
    BatteryRealData.Vcell[2].val=U16_SWAP(BatteryData.VCell3.VCell3Word);
    BatteryRealData.Vcell[3].val=U16_SWAP(BatteryData.VCell4.VCell4Word);
    BatteryRealData.Vcell[4].val=U16_SWAP(BatteryData.VCell5.VCell5Word);
    BatteryRealData.Vcell[5].val=U16_SWAP(BatteryData.VCell6.VCell6Word);
    BatteryRealData.Vcell[6].val=U16_SWAP(BatteryData.VCell7.VCell7Word);

    // 20190521  AID
    if(BatteryData.CC.CCWord< 0x8000)
    {
        BatteryRealData.Curr[0].val=U16_SWAP(BatteryData.CC.CCWord);
        BatteryRealData.Curr[1].val =0;
    }
    else
    {
        BatteryRealData.Curr[0].val=0;
        temp16 = BatteryData.CC.CCWord;                         //20190528 AID
        temp16 = 0-temp16;
        BatteryRealData.Curr[1].val =U16_SWAP(temp16);
    }

//    BatteryRealData.TempNum=2;

//    BatteryRealData.Temp[1] = TSDataCompute(BQ76930Data.TS1.TS1Byte.TS1_HI,BQ76930Data.TS1.TS1Byte.TS1_LO);   //fenglong 20190605
//    BatteryRealData.Temp[0] = TSDataCompute(BQ76930Data.TS2.TS2Byte.TS2_HI,BQ76930Data.TS2.TS2Byte.TS2_LO);   //fenglong 20190605

//    temp16=U16_SWAP(BQ76930Data.TS1.TS1Word);
//    BatteryRealData.Temp[1] =vAPI_IndexNtcTemp(temp16)+40;              //fenglong 20190605     温度还需验证
//    temp16=U16_SWAP(BQ76930Data.TS2.TS2Word);
//    BatteryRealData.Temp[0] =vAPI_IndexNtcTemp(temp16)+40;              //fenglong 20190605     温度还需验证

    BatteryRealData.Temp[1] = vAPI_IndexNtcTemp(BatteryData.TS1.TS1Word)+40;         //fenglong 20190607
    BatteryRealData.Temp[0] = vAPI_IndexNtcTemp(BatteryData.TS2.TS2Word)+40;          //fenglong 20190607

    BatteryRealData._VSTATE.val=0;
    if((HW_PT_Status.Bits.HW_OV)||(FW_PT_Status.Bits.PT_OV))
    {
//        BatteryRealData._VSTATE.bit.VOV=1;        //cell over voltage 单体过压      //fenglong 20190606       //20190621 客户提出不上传
    }
//    BatteryRealData._VSTATE.bit.VOV = FW_PT_Status.Bits.PT_OV;
    if((FW_PT_Status.Bits.PT_UV)||(HW_PT_Status.Bits.HW_UV))
    {
        BatteryRealData._VSTATE.bit.VUV=1;        //cell under voltage      //fenglong 20190606
    }
//    BatteryRealData._VSTATE.bit.VUV = FW_PT_Status.Bits.PT_UV;


    if(CellMiniVoltage<=ALARM_UV_Default)
    {
        BatteryRealData._VSTATE.bit.wVUV=1;
    }

//    if(CellMAXiVoltage>=(ALARM_OV_Default-70))            //fenglong 20190621
//    {
//        BatteryRealData._VSTATE.bit.wVOV=1;
//    }



//    BatteryRealData._VSTATE.bit.wBVOV = ALARM_PT_Status.Bits.wBVOV ;        //fenglong 20190606
    BatteryRealData._VSTATE.bit.wBVUV = ALARM_PT_Status.Bits.wBVUV ;        //fenglong 20190606

    BatteryRealData._VSTATE.bit.VDIFF = FW_PF_Status.Bits.PF_VIMA;

//    BatteryRealData._VSTATE.bit.VBREAK=FW_PF_Status.Bits.PF_OTC;        //permanent fail safety over temperature cell

    if(FW_PF_Status.Word != 0)
    {
        BatteryRealData._VSTATE.bit.VBREAK=1;
    }
    else
    {
        BatteryRealData._VSTATE.bit.VBREAK=0;
    }

//    BatteryRealData._VSTATE.bit.CSGDIS=1;

//    BatteryRealData._VSTATE.val=0b1111110111111000;             //for test fenglong 20190612

    BatteryRealData._VSTATE.val=U16_SWAP(BatteryRealData._VSTATE.val);



    BatteryRealData._CSTATE.val=0;


    if(BatteryData.CC.CCWord >= 0x8000)
    {
        temp16 = 0 - BatteryData.CC.CCWord ;

        if(temp16 >= 50)
        {
            BatteryRealData._CSTATE.bit.CING=0;
            BatteryRealData._CSTATE.bit.DING=1;
        }
    }
    else if(BatteryData.CC.CCWord > 50)
    {
        BatteryRealData._CSTATE.bit.CING=1;
        BatteryRealData._CSTATE.bit.DING=0;
    }



    BatteryRealData._CSTATE.bit.OCCSG=FW_PF_Status.Bits.PF_OCC;//FW_PF_Status.Bits.PF_OTC;     //permanent fail safety over temperature cell
    BatteryRealData._CSTATE.bit.SHORT=HW_PT_Status.Bits.HW_SCD;     //short current detection  短路监测电流
    BatteryRealData._CSTATE.bit.OCDSG1=FW_PT_Status.Bits.PT_ODP;    //over discharge current protection
    BatteryRealData._CSTATE.bit.OCDSG2=HW_PT_Status.Bits.HW_OCD;

//    BatteryRealData._CSTATE.bit.wOCCSG = FW_PT_Status.Bits.PT_OCP ;
//    BatteryRealData._CSTATE.bit.wOCDSG = FW_PT_Status.Bits.PT_ODP ;

//    BatteryRealData._CSTATE.val=0b1111111111011110;             //for test fenglong 20190612

    BatteryRealData._CSTATE.val=U16_SWAP(BatteryRealData._CSTATE.val);



    BatteryRealData._TSTATE.val=0;

    BatteryRealData._TSTATE.bit.TCELL_CSGH=FW_PT_Status.Bits.PT_OTCP;
    BatteryRealData._TSTATE.bit.TCELL_CSGL=FW_PT_Status.Bits.PT_UTCP;
    BatteryRealData._TSTATE.bit.TCELL_DSGH=FW_PT_Status.Bits.PT_OTDP;
    BatteryRealData._TSTATE.bit.TCELL_DSGL=FW_PT_Status.Bits.PT_UTDP;

    BatteryRealData._TSTATE.bit.TENV_H=FW_PT_Status.Bits.PT_OTSCP;          // Environmental high temperature warning                 fenglong 20190602
    BatteryRealData._TSTATE.bit.TENV_L=FW_PT_Status.Bits.PT_UTSCP;          // Environmental low temperature warning                  fenglong 20190602

    BatteryRealData._TSTATE.bit.TFET_H=(FW_PF_Status.Bits.PF_OTM || ALARM_PT_Status.Bits.HW_OTM);        //fenglong 20190705
//    BatteryRealData._TSTATE.bit.TFET_L=1;

    BatteryRealData._TSTATE.bit.wTCELL_H=ALARM_PT_Status.Bits.AL_OTDA;        // cell high temperature warning                 fenglong 20190602
                                            //FW_PF_Status.Bits.PF_OTC
    BatteryRealData._TSTATE.bit.wTCELL_L=ALARM_PT_Status.Bits.AL_UTDA;
//    BatteryRealData._TSTATE.bit.wTENV_H=0;
//    BatteryRealData._TSTATE.bit.wTENV_L=1;
    BatteryRealData._TSTATE.bit.wTFET_H=ALARM_PT_Status.Bits.AL_OTMA;
//    BatteryRealData._TSTATE.bit.wTFET_L=1;

//    BatteryRealData._TSTATE.val=0b1111111111110101;                       //for test fenglong 20190612
    BatteryRealData._TSTATE.val=U16_SWAP(BatteryRealData._TSTATE.val);

  //  BatteryRealData.SOC=SW_Status.;

    BatteryRealData._ALARM.val=0;

//    BatteryRealData._ALARM.bit.bit0=1;
//    BatteryRealData._ALARM.bit.bit1=0;
//    BatteryRealData._ALARM.bit.bit2=1;
//
//    BatteryRealData._ALARM.bit.SPI_ERR=0;
//    BatteryRealData._ALARM.bit.E2PROM_ERR=0;
//    BatteryRealData._ALARM.bit.bit5=0;
//    BatteryRealData._ALARM.bit.FCC_UPDATING=1;
//    BatteryRealData._ALARM.bit.FCC_DSGLEARN=1;

    BatteryRealData._ALARM.val=U16_SWAP(BatteryRealData._ALARM.val);


    BatteryRealData._FETSTATE.bit.DFET=BQ76930Data.SysCtrl2.SysCtrl2Bit.DSG_ON;
    BatteryRealData._FETSTATE.bit.CFET=BQ76930Data.SysCtrl2.SysCtrl2Bit.CHG_ON;
//    BatteryRealData._FETSTATE.bit.SDFET=0;
//    BatteryRealData._FETSTATE.bit.SCFET=0;
//    BatteryRealData._FETSTATE.bit.DFET_DAMAGE=1;
//    BatteryRealData._FETSTATE.bit.CFET_DAMAGE=0;
//    BatteryRealData._FETSTATE.bit.CCFET=0;

//    BatteryRealData.NUM_VOV.val=U16_SWAP(0x7890);
//
//    BatteryRealData.NUM_VUV.val=U16_SWAP(0x1122);
//
//    BatteryRealData.NUM_WARN_VHIGH.val=U16_SWAP(0x3344);
//    BatteryRealData.NUM_WARN_VLOW.val=U16_SWAP(0x5566);
//
//    BatteryRealData.BlanceState.len_byte.high=BQ76930Data.CellBal1.CellBal1Byte;        //fenglong 20190620
//    BatteryRealData.BlanceState.len_byte.low=BQ76930Data.CellBal2.CellBal2Byte;         //fenglong 20190620
//    BatteryRealData.BlanceState.val= U16_SWAP(BatteryRealData.BlanceState.val);         //fenglong 20190620
//    if( ((BQ76930Data.CellBal1.CellBal1Byte & 0x1F) != 0) || ((BQ76930Data.CellBal2.CellBal2Byte & 0x1F) != 0) )
//    {
//        BatteryRealData.BlanceState = BQ76930Data.CellBal1.CellBal1Byte&0x07;
//        if(BQ76930Data.CellBal1.CellBal1Byte&0x10)
//        {
//            BatteryRealData.BlanceState += 0x08;
//        }
//        if(BQ76930Data.CellBal2.CellBal2Byte &0x01)
//        {
//            BatteryRealData.BlanceState += 0x10;
//        }
//        if(BQ76930Data.CellBal2.CellBal2Byte &0x02)
//        {
//            BatteryRealData.BlanceState += 0x20;
//        }
//        if(BQ76930Data.CellBal2.CellBal2Byte &0x10)
//        {
//            BatteryRealData.BlanceState += 0x40;
//        }
//        //BatteryRealData.BlanceState = MAXVoltage_Num;
//    }
//    else
//    {
//        BatteryRealData.BlanceState = 0;
//    }

//    BatteryRealData.DchgNum.val=U16_SWAP(his_data[CHG_CAP_CNT]);
//
//    BatteryRealData.DchgNum.val=U16_SWAP(his_data[FLASH_CYCLES_ADDR]);
//    BatteryRealData.DchgNum.val=U16_SWAP(TOFF_DHG_MOS_Count);		//FENGLONG 20190805

    BatteryRealData.ChgNum.val=U16_SWAP(his_data[FLASH_CYCLES_ADDR]);

//    BatteryRealData.SOC=BatteryCapacity;      //20190528 fenglong

    BatteryRealData.SOC=sys_cap.val.re_cap_rate;                      //20190528 fenglong


    //Dis_SOC();

//    BatteryRealData.CapNow.val  = sys_cap.val.full_cap * sys_cap.val.re_cap_rate/100;   //可能需要更改    //fenglong 20190604

    BatteryRealData.CapNow.val  = U16_SWAP(sys_cap.val.bat_cap);   //fenglong 20190604
    BatteryRealData.CapFull.val = U16_SWAP(sys_cap.val.full_cap);




//    BatteryRealData.CapNow.val=U16_SWAP(65*BatteryCapacity);  //U16_SWAP(BatteryRealData.CapNow.val);      //20190528 fenglong
//    BatteryRealData.CapFull.val=U16_SWAP(65*100);     //U16_SWAP(BatteryRealData.CapFull.val);             //20190528 fenglong



//    BatteryRealData.CRC=crc8_cal((uint)&BatteryRealData,sizeof(BatteryRealData)-1);         //0xF2;
    //主机收到CRC=0XFA,,,   我发的是0X09
}



void Dis_SOC(void)
{

	if(SystemMode==SystemChargeMode)
	{
		if( ((BatteryData.VBat.VBatWord >=CHG_VOL_FULL) && (BatteryData.CC.CCWord < CHG_CC_FULL)) || ( HW_PT_Status.Bits.HW_OV == 1 ) || (HW_PT_Status.Bits.HW_OV == 1) )
		{
			BatteryRealData.SOC = 100 ;

		}
		else
		{
//			BatteryRealData.SOC=BatteryCapacity;
		    BatteryRealData.SOC = sys_cap.val.re_cap_rate;                      //20190528 fenglong
		}
	}
	else if(SystemMode==SystemDischargeMode)
	{
		    BatteryRealData.SOC = sys_cap.val.re_cap_rate;                      //20190528 fenglong
	}
	else
	{
	    if((BatteryData.VBat.VBatWord >=CHG_VOL_FULL)&&(BatteryRealData.SOC == 100))
        {
            BatteryRealData.SOC = 100 ;
        }
        else
        {
//          BatteryRealData.SOC=BatteryCapacity;
            BatteryRealData.SOC = sys_cap.val.re_cap_rate;                      //20190528 fenglong
        }
	}
}



uint U16_SWAP(uint data)
{
    uint temp;
    temp=data>>8;
    temp|=(data & 0x00ff)<<8;
    return temp;
}


uchar ASC2HEX(uchar data1, uchar data2)
{
    uchar temp;

    if(data1 > 0x39)
    {
        data1-=0x37;
    }
    else
    {
        data1-=0x30;
    }

    if(data2 > 0x39)
    {
        data2-=0x37;
    }
    else
    {
        data2-=0x30;
    }

    temp=data1<<4 ;
    temp  = temp + data2;
    return temp;
}














