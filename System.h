/*
 * System.h
 *
 *  Created on: 20193る30ら
 *      Author: ET
 */


#ifndef __SYSTEM_H
#define __SYSTEM_H

//#define UART_PRINTF

//typedef bit                     BIT;
typedef unsigned char           UINT8;
typedef unsigned int            UINT16;
typedef unsigned long           UINT32;
typedef signed short            SINT16;

typedef unsigned char           uchar;
typedef unsigned int            uint;
typedef unsigned long           ulong;


typedef signed short              s16;
typedef unsigned short            u16;
typedef unsigned char             u8;
typedef signed char               s8;
typedef unsigned long             u32;



#include <msp430.h>
#include "stdint.h"
#include <stdio.h>

//#include "Table.h"
#include"uart.h"
#include "Initial.h"
#include "Subprogram.h"
#include "I2C.h"
#include "drv_USI_IIC.h"
#include "bqMaximo_Ctrl_G2553.h"
#include"soft_soc.h"
#include"uart.h"
#include"RS_485.h"
#include "ProtectRenew.h"
#include "protect.h"
#include "sleep.h"
//#include "ModeShift.h"
#include "timer.h"
#include "alarm.h"
#include "mosfet.h"
#include "calculate.h"
#include "TempCK.h"
#include "soft_soc.h"
//#include "Flash.H"
#include "balance.h"
#include "led.h"
#include "bq_app.h"
#include "read_key.h"
#include "count.h"
#define FLASH_CHECK    0x0820

#define Function_Enable     1
#define Function_Disable    0
#define SW_High             1



#define Segment_D       0x1000
#define Segment_C       0x1040
#define Segment_B       0x1080
#define Segment_A       0x10C0

#define Breathing_1600  80
#define Breathing_1200  60
//#define Breathing_4     4
//#define Breathing_3     3
//#define Breathing_2     2
//#define Breathing_1     1


#define EN_UART_RX_P11             (P1DIR  |= BIT2);(P1SEL|=BIT1);(P1SEL2|=BIT1)
#define EN_UART_TX_P12             (P1DIR  &=~BIT1);(P1SEL|=BIT2);(P1SEL2|=BIT2)

#define EN_I2C_SCL_P16             (P1SEL|=BIT6);(P1SEL2|=BIT6)
#define EN_I2C_SDA_P17             (P1SEL|=BIT7);(P1SEL2|=BIT7)
#define EN_P16_OUT                 (P1DIR|=BIT6);(P1SEL&=~BIT6);(P1SEL2&=~BIT6)
#define EN_P17_OUT                 (P1DIR|=BIT7);(P1SEL&=~BIT7);(P1SEL2&=~BIT7)
#define P16_High                   (P1OUT&=~BIT6)
#define P17_High                    (P1OUT&=~BIT7)

//  Define GPIO For Input
#define EN_SW_pin               (P1DIR&=~BIT3)
#define EN_DC_IN_pin            (P1DIR&=~BIT4)
#define EN_76930_Alert_Pin      (P1DIR&=~BIT5)

#define EN_WakeUP_Pin           (P2DIR&=~BIT3)

#define EN_RT_ON_Pin            (P2DIR|=BIT0)
#define RT_ON_HIGH              (P2OUT|=BIT0)
#define RT_ON_LOW               (P2OUT&=~BIT0)


//  Define GPIO For Output
#define EN_RE_485_Pin           (P3DIR|=BIT0)
#define EN_CP_Pin               (P3DIR|=BIT4)
#define EN_DE_485_Pin           (P3DIR|=BIT1)
#define EN_MCU_SAFE_Pin         (P3DIR|=BIT6)

#define EN_ALERT_Pin			(P1DIR &= ~BIT5);(P1IES &= ~BIT5);(P1IFG &= ~BIT5);(P1IE |= BIT5)
#define EN_DC_Pin				(P1DIR &= ~BIT4);(P1IES &= ~BIT4);(P1IFG &= ~BIT4);(P1IE |= BIT4)
#define EN_KEY_Pin				(P1DIR &= ~BIT3);(P1IES &= ~BIT3);(P1IFG &= ~BIT3);(P1IE |= BIT3)

//#define DIS_ALERT_Pin

#define MCU_SAFE_High           (P3OUT|=BIT6)
#define MCU_SAFE_Low            (P3OUT&=~BIT6)
#define CP_High                 (P3OUT|=BIT4)
#define CP_Low                  (P3OUT&=~BIT4)

#define DE_485_High             (P3OUT|=BIT1)
#define DE_485_Low              (P3OUT&=~BIT1)

#define RE_485_High             (P3OUT|=BIT0)
#define RE_485_Low              (P3OUT&=~BIT0)

#define RS485_DE              (P3OUT|=BIT0);(P3OUT|=BIT1)
#define RS485_RE             (P3OUT&=~BIT0); (P3OUT&=~BIT1)


#define KEY_UP                  ((P1IN & BIT3) != BIT3 )
#define KEY_DOWN                ((P1IN & BIT3) == BIT3 )

#define DC_IN					((P1IN & BIT4) != BIT4 )
#define DC_OUT					((P1IN & BIT4) == BIT4 )

#define ALERT_DIS					((P1IN & BIT5) != BIT5 )
#define ALERT_ON					((P1IN & BIT5) == BIT5 )

#define RS485_BUSY					((P2IN & BIT3) != BIT3 )
#define RS485_IDLE					((P2IN & BIT3) == BIT3 )

#define EN_PWM_LID_1_P21             (P2DIR|=BIT1);(P2SEL|=BIT1);(P2SEL2&=~BIT1)
#define EN_PWM_LID_2_P22             (P2DIR|=BIT2);(P2SEL|=BIT2);(P2SEL2&=~BIT2)
#define EN_PWM_LID_3_P33             (P3DIR|=BIT3);(P3SEL|=BIT3);(P3SEL2&=~BIT3)    //20190717 AID
#define EN_PWM_LID_4_P32             (P3DIR|=BIT2);(P3SEL|=BIT2);(P3SEL2&=~BIT2)    //20190717 AID
#define EN_PWM_LID_5_P24             (P2DIR|=BIT4);(P2SEL|=BIT4);(P2SEL2&=~BIT4)
#define EN_PWM_LID_6_P25             (P2DIR|=BIT5);(P2SEL|=BIT5);(P2SEL2&=~BIT5)

#define EN_LED_1_P21            (P2DIR|=BIT1);(P2SEL&=~BIT1);(P2SEL2&=~BIT1)
#define EN_LED_2_P22            (P2DIR|=BIT2);(P2SEL&=~BIT2);(P2SEL2&=~BIT2)
#define EN_LED_3_P33            (P3DIR|=BIT3);(P3SEL&=~BIT3);(P3SEL2&=~BIT3)    //20190717 AID
#define EN_LED_4_P32            (P3DIR|=BIT2);(P3SEL&=~BIT2);(P3SEL2&=~BIT2)    //20190717 AID
#define EN_LED_5_P24            (P2DIR|=BIT4);(P2SEL&=~BIT4);(P2SEL2&=~BIT4)
#define EN_LED_6_P25            (P2DIR|=BIT5);(P2SEL&=~BIT5);(P2SEL2&=~BIT5)


#define LED_1_Low              (P2OUT|=BIT1)
#define LED_1_High               (P2OUT&=~BIT1)
#define LED_2_Low              (P2OUT|=BIT2)
#define LED_2_High               (P2OUT&=~BIT2)
//20190717 AID
#define LED_3_Low              (P3OUT|=BIT3)
#define LED_3_High               (P3OUT&=~BIT3)
#define LED_4_Low              (P3OUT|=BIT2)
#define LED_4_High               (P3OUT&=~BIT2)

#define LED_5_Low              (P2OUT|=BIT4)
#define LED_5_High               (P2OUT&=~BIT4)
#define LED_6_Low              (P2OUT|=BIT5)
#define LED_6_High               (P2OUT&=~BIT5)


//#define EN_ACLK             (P1DIR|=BIT0);(P1SEL|=BIT0);(P1SEL2&=~BIT0)
//#define EN_SMCLK            (P1DIR|=BIT4);(P1SEL|=BIT4);(P1SEL2&=~BIT4)





#define Clear_TA0CCTL0_CCIFG    (TA0CCTL0&=~CCIFG)
#define Clear_TA0CCTL1_CCIFG    (TA0CCTL1&=~CCIFG)
#define Clear_TA0CCTL2_CCIFG    (TA0CCTL2&=~CCIFG)

#define Clear_TA1CCTL0_CCIFG    (TA1CCTL0&=~CCIFG)
#define Clear_TA1CCTL1_CCIFG    (TA1CCTL1&=~CCIFG)
#define Clear_TA1CCTL2_CCIFG    (TA1CCTL2&=~CCIFG)


#define TA0_20ms                    0x7500




//#define BQ76930LoadErrorLimit       50
//#define BQ76930LoadTrue             3

//#define LEDSpace            3

//SystemMode
#define SystemSleepMode         1

#define SystemBootUpWait        2
#define SystemBootUp            3
#define SystemShipMode          4
#define SystemChargeMode        5
#define SystemDischargeMode     6
#define SystemShutDown			7



//#define CellMiniVoltageLimit    0x0200          //not use
//#define CellNumber              7
//#define BQ76930_TS_Const        382
//#define BQ76930_25Deg_Const     1200000         //not use
//#define VoltageChangeTemp_Const 4200            //not use
//#define CurrentOffset           0xFEDE      //0.0A (- 0x0123)

//#define BQ76930_CC_Data_to_Current_Const    3376        // (8.44uV/2.5mR) X 1000 = uA
//#define CurrentD2AUnit          844
//#define CurrentShuntResister    250
//#define mA_10mA                 10


//#define BQ76930SaveFlashLength  11
//#define BQ76930ADCBase          365

#define BQ76930_Address             0x08
//#define DELAY_2ms_MCLK12Mhz         0x1200

//#define BQ76930_DataLength          0x34
#define BQ76930_SysState_Addr       0x00

#define BQ76930_SYS_CTRL1_Addr      0x04        //可以不要相關部分
#define BQ76930_SYS_CTRL2_Addr      0x05
//#define BQ76930_ADCGAIN1_Addr       0x50
//#define BQ76930_ADCGAIN2_Addr       0x59
//#define BQ76930_ADCOFFSET_Addr      0x51

//#define Write_BQ76930DataLength     7       //not use

//#define BQ76930CommunicationError   0x10
//#define BQ76930DataError            0x11

#define BQ76930_SYS_CTRL1_Default   0x18            //0x04          需要查下數據意義ok
#define BQ76930_SYS_CTRL2_Default   0x40            //0x05          需要查下數據意義ok
#define BQ76930_PROTECT1_Default    0x8D             //0x06          需要查下數據意義       ok
#define BQ76930_PROTECT2_Default    0x29             //0x07          需要查下數據意義       ok  short current in discharge threshold 67mV
#define BQ76930_PROTECT3_Default    0x00      //OV,UV Delay 1S fenglong 20190612       //0x08          需要查下數據意義       OK
#define BQ76930_OV_TRIP_Default     0xC9      //(4.3V)    0x09          需要查下數據意義
#define BQ76930_UV_TRIP_Default     0x1A      //(2.5V)    0x0A          需要查下數據意義

#define BQ76930_CHG_ON              0x01
#define BQ76930_DSG_ON              0x02

#define PF_OV_Default           4350        //mV            //4.23mV    -   8s      //permanent fail safety over voltage
#define PF_UV_Default           2000        //mV                //permanent fail safety under voltage
#define PF_OCC_Default          6000        //1600        //16.00 A           //permanent fail  over current in charge fenglong 20190603
#define PF_OCD_Default          32000       //5000    //-50.00 A //permanent fail  over current in discharge

#define PF_OTC_Default          880         //95 Deg      //fenglong 20190607      //permanent fail safety over temperature cell
#define PF_OTM_Default          610         //111 Deg //120 Deg     //fenglong 20190607      //permanent fail safety over temperature MOSFET

#define PT_OV_Default           4060        //mV    (BQ76930 Data)
#define PT_OV_Release           3980        //mV        //

#define PT_UV_Default           2800        //2.8V
#define PT_UV_Release           3400       //3100        //3.1V

#define PT_OCP_Default          1500		//2600     //1260        //12.60 A           //over charge current protection
#define PT_ODP_Default          3000		//25000	//fenglong 20190801 应客户要求修改  23000    //3300        //-33.00 A          //over discharge current protection  20190522 設置22安，實際19.6安，

//V = NTC10K+10K  3.3V   AD = (RT/RT+10000)*3300*4096/3300
//VTSX = AD*382 μV
//RTS = 10000*VTSX/(3.3-VTSX)
//VTSX = 3.3RTS/(10000+RTS)
//AD = 3.3RTS/(10000+RTS)/382
#define PT_OTCP_Default         2484  //50 Deg  fenglong 20190606          //over temperature charge protection
#define PT_OTCP_Release         3110  //40 Deg  fenglong 20190606

#define ALARM_OTDA_Default      1760    //65 Deg
#define PT_OTDP_Default         1393    //75 Deg    fenglong 20190606        //over temperature discharge protection
#define PT_OTDP_Release         2222    //55 Deg    fenglong 20190606

#define ALARM_UTDA_Default      7123
#define PT_UTCP_Default         6377  //0 Deg             //under temperature charge protection
#define PT_UTCP_Release         5775  //7 Deg
#define PT_UTDP_Default         7516  //-18 Deg           //under temperature discharge protection
#define PT_UTDP_Release         7068  //-10 Deg

#define ALARM_OTMA_Default      1182    //82 Deg


//#define UV_Default              2600        //mV    (BQ76930 Data)

#define OV_DataBase             0x2008
#define UV_DataBase             0x1000
#define OV_MiniData             0x9F        //(4.1v)
#define UV_MaxiData             0x1A        //(2.5V)

//#define Charge_LED_Mode_0       0x00
#define Charge_LED_Mode_1       0x01
#define Charge_LED_Mode_2       0x02
#define Charge_LED_Mode_3       0x03
#define Charge_LED_Mode_4       0x04


#define Discharge_LED_Mode_1    0x10    //0~15%
#define Discharge_LED_Mode_2    0x20    //15~30%
#define Discharge_LED_Mode_3    0x30    //30~50%
#define Discharge_LED_Mode_4    0x40    //50~70%
#define Discharge_LED_Mode_5    0x50    //70~85%
#define Discharge_LED_Mode_6    0x60    //85~99%




#define SYS_STAT 0
#define CELLBAL1 1
#define CELLBAL2 2
#define CELLBAL3 3
#define SYS_CTRL1 4
#define SYS_CTRL2 5
#define PROTECT1 0x06
#define PROTECT2 0x07
#define PROTECT3 0x08
#define OV_TRIP 0x09
#define UV_TRIP 0x0A
#define VC1_HI_BYTE 0x0C
#define VC6_HI_BYTE 0x16
#define BAT_HI_BYTE 0x2A
#define TS1_HI_BYTE 0x2C
#define CC_HI_BYTE  0x32


#define ADCGAIN1 0x50
#define ADCOFFSET 0x51
#define ADCGAIN2 0x59

#define SCD_DELAY_50us		0x0
#define SCD_DELAY_100us		0x1
#define SCD_DEALY_200us		0x2
#define SCD_DELAY_400us		0x3

#define SCD_THRESH_44mV_22mV	0
#define SCD_THRESH_67mV_33mV	1
#define SCD_THRESH_89mV_44mV	2
#define SCD_THRESH_111mV_56mV	3
#define SCD_THRESH_133mV_67mV	4
#define SCD_TRHESH_155mV_68mV	5
#define SCD_THRESH_178mV_89mV	6
#define SCD_THRESH_200mV_100mV	7

#define OCD_DEALY_10ms		0x0
#define OCD_DELAY_20ms		0x1
#define OCD_DELAY_40ms		0x2
#define OCD_DELAY_80ms		0x3
#define OCD_DELAY_160ms		0x4
#define OCD_DELAY_320ms		0x5
#define OCD_DELAY_640ms		0x6
#define OCD_DELAY_1280ms	0x7

#define OCD_THRESH_17mV_8mV		0
#define OCD_THRESH_22mV_11mV	1
#define OCD_THRESH_28mV_14mV	2
#define OCD_THRESH_33mV_17mV	3
#define OCD_THRESH_39mV_19mV	4
#define OCD_THRESH_44mV_22mV	5
#define OCD_THRESH_50mV_25mV	6
#define OCD_THRESH_56mV_28MV	7
#define OCD_THRESH_61mV_31mV	8
#define OCD_THRESH_67mV_33mV	9
#define OCD_THRESH_72mV_36mV	0xA
#define OCD_THRESH_78mV_39mV	0xB
#define OCD_THRESH_83mV_42mV	0xC
#define OCD_THRESH_89mV_44mV	0xD
#define OCD_THRESH_94mV_47mV	0xE
#define OCD_THRESH_100mV_50mV	0xF

#define UV_DELAY_1s				0
#define UV_DELAY_4s				1
#define UV_DELAY_8s				2
#define UV_DELAY_16s			3

#define OV_DELAY_1s				0
#define OV_DELAY_2s				1
#define OV_DELAY_4s				2
#define OV_DELAY_8s				3

#define OV_THRESH_BASE			0x2008
#define UV_THRESH_BASE			0x1000
#define OV_STEP					0x10
#define UV_STEP					0x10


#define FLASH_CHECK_ADDR        0
#define PF_STATE_ADDR           1
#define HIS_MAXVOLT             2
#define HIS_MINVOLT             3
#define HIS_CHG_MAXCURRENT      4
#define HIS_DSG_MAXCURRENT      5
#define HIS_CHG_MAXTEMP         6
#define HIS_DSG_MAXTEMP         7

#define HIS_MOSFET_MAXTEMP      8       //fenglong 20190614
#define HIS_DSG_UVP_ADD         9
#define FLASH_CYCLES_ADDR       10
#define HIS_Cell1MAXiVoltage        11
#define HIS_Cell1MiniVoltage        12
#define HIS_Cell2MAXiVoltage        13
#define HIS_Cell2MiniVoltage        14
#define HIS_Cell3MAXiVoltage        15
#define HIS_Cell3MiniVoltage        16
#define HIS_Cell4MAXiVoltage        17
#define HIS_Cell4MiniVoltage        18
#define HIS_Cell5MAXiVoltage        19
#define HIS_Cell5MiniVoltage        20
#define HIS_Cell6MAXiVoltage        21
#define HIS_Cell6MiniVoltage        22
#define HIS_Cell7MAXiVoltage        23
#define HIS_Cell7MiniVoltage        24
#define PRODUCTION_DDMM             25
#define PRODUCTION_YY               26      //用一个字节，剩余一个字节
#define CHG_CAP_CNT					27		//用一个字节，剩余一个字节

#define HIS_CRC16               FLASH_DATA_NUM-1  //15  fenglong 20190614
#define FLASH_DATA_NUM          29      //32 words      fenglong20190617

#define CHG_VOL_FULL        29190
#define CHG_CC_FULL         100



struct _HWPTBits{
    UINT8   HW_OCD:1;           //放电过流2级保护
    UINT8   HW_SCD:1;           //short current detection  短路监测电流
    UINT8   HW_OV:1;            //cell over voltage 单体过压
    UINT8   HW_UV:1;            //cell under voltage 单体过流

};
union HW_PT{
    UINT8   Byte;
    struct _HWPTBits Bits;
};

struct _SW_ALARM_Bits{
    UINT8   HW_OTM:1;           //90 Dec   -   4s      //over temperature MOSFET
    UINT8   wBVOV:1;        //fenglong 20190606
    UINT8   wBVUV:1;        //fenglong 20190606
    UINT8   AL_OTDA:1;        //fenglong 20190606
    UINT8   AL_UTDA:1;        //fenglong 20190606
    UINT8   AL_OTMA:1;        //fenglong 20190606
    UINT8   AL_OV:1;        //fenglong 20190621
    UINT8   AL_GLINT:1;     // fenglong 20190702 报警灯闪烁
};
union SW_ALARM{
    UINT8   Byte;
    struct _SW_ALARM_Bits Bits;
};

struct _FWPFBits {//permanent fail    //永久烧毁保护
    UINT8   PF_OCC:1;           //16A   -   4s          //permanent fail  over current in charge
    UINT8   PF_OCD:1;           //50A   -   4s          //permanent fail  over current in discharge
    UINT8   PF_OV:1;            //4.23mV    -   8s      //permanent fail safety over voltage
    UINT8   PF_UV:1;            //1.50mV    -   8s      //permanent fail safety under voltage
    UINT8   PF_OTC:1;           //95 Dec    -   4s      //permanent fail safety over temperature cell
    UINT8   PF_OTM:1;           //120 Dec   -   4s      //permanent fail safety over temperature MOSFET
    UINT8   PF_CMF:1;           //0.2A  -   20s         //permanent fail charge MOSFET fail
    UINT8   PF_DMF:1;           //-0.2A -   20s         //permanent fail discharge MOSFET fail

    UINT8   PF_VIMR:1;                                  //permanent fail Voltage imbalance at rest check voltages   //电压不平衡
    UINT8   PF_VIMA:1;                                  //permanent fail Voltage imbalance at rest Delta delay time
};
union FW_PF{
    UINT16   Word;
    struct _FWPFBits Bits;
};

struct _PTBits {
    UINT8   PT_OV:1;                                                //over voltage protection
    UINT8   PT_UV:1;            //2.6V                              //under voltage protection
    UINT8   PT_OCP:1;           //12.6A-2s                          //over charge current protection
    UINT8   PT_ODP:1;           //33.0A-5s          //放电过流1级保护             //over discharge current protection
    UINT8   PT_OTCP:1;           //48 Dec - 43 Dec                  //over temperature charge protection
    UINT8   PT_OTDP:1;           //68 Dec - 63 Dec                  //over temperature discharge protection
    UINT8   PT_UTCP:1;           //2 Dec - 7 Dec                    //under temperature charge protection
    UINT8   PT_UTDP:1;           //-18 Dec - -13 Dec                //under temperature discharge protection

    UINT8   PT_OTSCP:1;     //  over temperature protection befor charge            fenglong 20190531
    UINT8   PT_UTSCP:1;     //  under temperature protection befor charge            fenglong 20190531

    UINT8   PT_OTSDP:1;     //  over temperature protection befor charge            fenglong 20190531
    UINT8   PT_UTSDP:1;     //  under temperature protection befor charge            fenglong 20190531



};
union FW_PT{
    UINT16   Word;          //fenglong 20190531
    struct _PTBits Bits;
};

struct T1{
    UINT8   Default:1;
    UINT8   FLASH:1;
    UINT8   Customer:1;
//    UINT8   NotData:1;
};
union _DataSource{
    UINT8   Byte;
    struct T1 Bits;
};

struct _SW_{
    UINT8   SW_Press:1;
    UINT8   SW_Release:1;
    UINT8   SW_Action:1;
    UINT8   SW_CKSOC:1;
};

union _SW
{
    struct _SW_ bits;
    UINT8 byte;
};




struct R1{
    UINT8 LO;
    UINT8 HI;
};
union _WORD{
    UINT16   Word;
    struct R1 Byte;
};

struct E1{
    UINT16 Lo;
    UINT16 Hi;
};
union _LONG{
    UINT32   Long;
    struct E1 Word;
};






typedef struct _BatteryData_
{
    union
    {
        struct
        {
            unsigned char VC1_LO;
            unsigned char VC1_HI;
        }VCell1Byte;
        unsigned short VCell1Word;
    }VCell1;

    union
    {
        struct
        {
            unsigned char VC2_LO;
            unsigned char VC2_HI;
        }VCell2Byte;
        unsigned short VCell2Word;
    }VCell2;

    union
    {
        struct
        {
            unsigned char VC3_LO;
            unsigned char VC3_HI;
        }VCell3Byte;
        unsigned short VCell3Word;
    }VCell3;

    union
    {
        struct
        {
            unsigned char VC4_LO;
            unsigned char VC4_HI;
        }VCell4Byte;
        unsigned short VCell4Word;
    }VCell4;

    union
    {
        struct
        {
            unsigned char VC5_LO;
            unsigned char VC5_HI;
        }VCell5Byte;
        unsigned short VCell5Word;
    }VCell5;

    union
    {
        struct
        {
            unsigned char VC6_LO;
            unsigned char VC6_HI;
        }VCell6Byte;
        unsigned short VCell6Word;
    }VCell6;

    union
    {
        struct
        {
            unsigned char VC7_LO;
            unsigned char VC7_HI;
        }VCell7Byte;
        unsigned short VCell7Word;
    }VCell7;

    union
    {
        struct
        {
            unsigned char BAT_LO;
            unsigned char BAT_HI;
        }VBatByte;
        unsigned short VBatWord;
    }VBat;

    union
    {
        struct
        {
            unsigned char TS1_LO;
            unsigned char TS1_HI;
        }TS1Byte;
        unsigned short TS1Word;
    }TS1;

    union
    {
        struct
        {
            unsigned char TS2_LO;
            unsigned char TS2_HI;
        }TS2Byte;
        unsigned short TS2Word;
    }TS2;

    union
    {
        struct
        {
            unsigned char CC_LO;
            unsigned char CC_HI;
        }CCByte;
        unsigned short CCWord;
    }CC;        //库仑计   coulomb counter
}_BatteryData;
//
//
//typedef struct _FlashData_
//{
//    union
//    {
//        struct
//        {
//            unsigned char OCD           :1;
//            unsigned char SCD           :1;
//            unsigned char OV            :1;
//            unsigned char UV            :1;
//            unsigned char OVRD_ALERT    :1;
//            unsigned char DEVICE_XREADY :1;
//            unsigned char WAKE          :1;
//            unsigned char CC_READY      :1;
//        }StatusBit;
//        unsigned char StatusByte;
//    }SysStatus;
//
//    union
//    {
//        struct
//        {
//            unsigned char CB1           :1;
//            unsigned char CB2           :1;
//            unsigned char CB3           :1;
//            unsigned char CB4           :1;
//            unsigned char CB5           :1;
//            unsigned char RSVD          :3;
//        }CellBal1Bit;
//        unsigned char CellBal1Byte;
//    }CellBal1;
//
//    union
//    {
//        struct
//        {
//            unsigned char CB6           :1;
//            unsigned char CB7           :1;
//            unsigned char CB8           :1;
//            unsigned char CB9           :1;
//            unsigned char CB10          :1;
//            unsigned char RSVD          :3;
//        }CellBal2Bit;
//        unsigned char CellBal2Byte;
//    }CellBal2;
//
//    union
//    {
//        struct
//        {
//            unsigned char CB11          :1;
//            unsigned char CB12          :1;
//            unsigned char CB13          :1;
//            unsigned char CB14          :1;
//            unsigned char CB15          :1;
//            unsigned char RSVD          :3;
//        }CellBal3Bit;
//        unsigned char CellBal3Byte;
//    }CellBal3;
//
//    union
//    {
//        struct
//        {
//            unsigned char SHUT_B        :1;
//            unsigned char SHUT_A        :1;
//            unsigned char RSVD1         :1;
//            unsigned char TEMP_SEL      :1;
//            unsigned char ADC_EN        :1;
//            unsigned char RSVD2         :2;
//            unsigned char LOAD_PRESENT  :1;
//        }SysCtrl1Bit;
//        unsigned char SysCtrl1Byte;
//    }SysCtrl1;
//
//    union
//    {
//        struct
//        {
//            unsigned char CHG_ON        :1;
//            unsigned char DSG_ON        :1;
//            unsigned char WAKE_T        :2;
//            unsigned char WAKE_EN       :1;
//            unsigned char CC_ONESHOT    :1;
//            unsigned char CC_EN         :1;
//            unsigned char DELAY_DIS     :1;
//        }SysCtrl2Bit;
//        unsigned char SysCtrl2Byte;
//    }SysCtrl2;
//
//    union
//    {
//        struct
//        {
//            unsigned char SCD_THRESH    :3;
//            unsigned char SCD_DELAY     :2;
//            unsigned char RSVD          :2;
//            unsigned char RSNS          :1;
//        }Protect1Bit;
//        unsigned char Protect1Byte;
//    }Protect1;
//
//    union
//    {
//        struct
//        {
//            unsigned char OCD_THRESH    :4;
//            unsigned char OCD_DELAY     :3;
//            unsigned char RSVD          :1;
//        }Protect2Bit;
//        unsigned char Protect2Byte;
//    }Protect2;
//
//    union
//    {
//        struct
//        {
//            unsigned char RSVD          :4;
//            unsigned char OV_DELAY      :2;
//            unsigned char UV_DELAY      :2;
//        }Protect3Bit;
//        unsigned char Protect3Byte;
//    }Protect3;
//
//    unsigned char OVTrip;
//    unsigned char UVTrip;
//    unsigned char CCCfg;
//} _FlashData;
#endif

