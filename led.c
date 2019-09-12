/*
 * led.c
 *
 *  Created on: 2019Äê7ÔÂ12ÈÕ
 *      Author: fenglong
 */


#include "System.h"
#include "led.h"

extern UINT8 SystemMode;
extern UINT8 SystemBootUpCount;
extern uchar RS485_MODE;
extern UINT8 SleepCount;
extern UINT8 LED_Mode;
extern UINT8 ChargeLED_Mode2_3_Count;

//extern UINT16 BatteryCapacityRefreshCount;
//volatile uchar led_real_rate[3];
//volatile uchar led_set_rate[3];
volatile uchar led1_real_rate=0;
volatile uchar led2_real_rate=0;
volatile uchar led3_real_rate=0;

volatile uchar led1_set_rate=0;
volatile uchar led2_set_rate=0;
volatile uchar led3_set_rate=0;
union _LED_CTRL LED_CTRL;

extern union FW_PF FW_PF_Status;       //  Èý¼¶±£»¤£¿
extern union FW_PT FW_PT_Status;       //CPU           Ò»¼¶±£»¤
extern union SW_ALARM ALARM_PT_Status;        //¾¯¸æÖµ×´Ì¬
extern UINT8 BreathingCycleCount;
extern const UINT16 BreathingTable_1600[];
extern const UINT16 BreathingTable_800[];
//extern const UINT16 BreathingTable_1200[];




void LEDForBootUpMode(void)
{
    if (SystemMode == SystemBootUp)
    {
        if(SystemBootUpCount ==0)
        {
        	LED_OFF();

            timer1_init();

        }
        switch (SystemBootUpCount)
        {
        	case 0:
        		LED_OFF();
        		break;

            case 0+2:
                LED_CTRL.Word = 0x0003;				//½¥½¥ÁÁ
                break;
            case 14+2:
                LED_CTRL.Word &= 0xFFFD;			//½¥½¥Ãð
                break;


            case 8+2:
                LED_CTRL.Word += 0x0030;			//½¥½¥ÁÁ
                break;
            case 22+2:
                LED_CTRL.Word &= 0xFFDF;			//½¥½¥Ãð
                break;

            case 16+2:
                LED_CTRL.Word += 0x0300;			//½¥½¥ÁÁ
                break;
            case 30+2:
                LED_CTRL.Word &= 0xFDFF;			//½¥½¥Ãð
                break;

            case 44+2:
            	LED_CTRL.Word = 0x0300;				//½¥½¥ÁÁ
            	break;
            case 44+8+2:
                LED_CTRL.Word += 0x0030;			//½¥½¥ÁÁ
                break;
            case 44+16+2:
                LED_CTRL.Word += 0x0003;			//½¥½¥ÁÁ
                break;

            case 60+14+8+2:
                LED_CTRL.Word &= 0xFDDD;			//½¥½¥Ãð
                break;

            case 60+14+8+6+2:
                LED_CTRL.Word =0;
                LED_OFF();
                close_timer1();
                break;

        }

        SystemBootUpCount = SystemBootUpCount + 1;

        if (SystemBootUpCount > 100)
        {
            RS485_MODE = 0 ;                                //fenglong 20190603
            Temp_DSG_Start_Check();                     //fenglong 20190709
            SystemMode = SystemDischargeMode;           //fenglong 20190709
            SleepCount=0;
        }
    }
}

void LED1_PWM( uchar LEDx)
{
    if( led1_real_rate < led1_set_rate )
    {
        P2OUT &= ~LEDx;
    }
    else
    {
        P2OUT |= LEDx;
    }
    led1_real_rate++;
    if(led1_real_rate>=100)
    {
        led1_real_rate=0;
    }
}

void LED2_PWM( uchar LEDx)
{
    if( led2_real_rate < led2_set_rate )
    {
        P2OUT &= ~LEDx;
    }
    else
    {
        P2OUT |= LEDx;
    }
    led2_real_rate++;
    if(led2_real_rate>=100)
    {
        led2_real_rate=0;
    }
}

void LED3_PWM( uchar LEDx)
{
    if( led3_real_rate < led3_set_rate )
    {
        P3OUT &= ~LEDx;
    }
    else
    {
        P3OUT |= LEDx;
    }
    led3_real_rate++;
    if(led3_real_rate>=100)
    {
        led3_real_rate=0;
    }
}


void LED_OFF(void)
{
    LED_1_Low;
    LED_2_Low;
    LED_3_Low;
    LED_4_Low;
    LED_5_Low;
    LED_6_Low;
    EN_LED_1_P21;
    EN_LED_2_P22;
    EN_LED_3_P33;
    EN_LED_4_P32;
    EN_LED_5_P24;
    EN_LED_6_P25;
}

void TA11_Breathing(void)
{
    UINT8 Buff;

    if( ALARM_PT_Status.Bits.AL_GLINT == 1)
    {

    	if(SystemMode == SystemChargeMode)
    	{
    		if( FW_PF_Status.Word != 0 )
    		{
    	        Buff=40;

    	        BreathingCycleCount = BreathingCycleCount + 1;
    	        if (BreathingCycleCount >= Buff)
    	        {
    	            BreathingCycleCount = 0;
    	        }
    	        TA1CCR1 = BreathingTable_800[BreathingCycleCount];// 20190718  AID
    	        TA1CCR2 = BreathingTable_800[BreathingCycleCount];
    		}
    		else if( (FW_PT_Status.Bits.PT_OTDP == 1) || (FW_PT_Status.Bits.PT_OTSCP == 1) || (FW_PT_Status.Bits.PT_OTCP == 1) )
    		{
    	        if (LED_Mode == Charge_LED_Mode_2)
    	            Charge_Mode2_LED_Display();
    	        if (LED_Mode == Charge_LED_Mode_3)
    	            Charge_Mode3_LED_Display();

    	        TA1CCR1 = BreathingTable_1600[BreathingCycleCount];
    	        TA1CCR2 = BreathingTable_1600[BreathingCycleCount];// 20190719  AID
    	        ChargeLED_Mode2_3_Count = ChargeLED_Mode2_3_Count + 1;
    	        BreathingCycleCount = BreathingCycleCount + 1;

    	        if ((LED_Mode == Charge_LED_Mode_1)
    	                && (BreathingCycleCount >= Breathing_1600))
    	            BreathingCycleCount = 0;
    		}
    		else
    		{
    	        Buff=40;

    	        BreathingCycleCount = BreathingCycleCount + 1;
    	        if (BreathingCycleCount >= Buff)
    	        {
    	            BreathingCycleCount = 0;
    	        }
    	        TA1CCR1 = BreathingTable_800[BreathingCycleCount];// 20190718  AID
    	        TA1CCR2 = BreathingTable_800[BreathingCycleCount];
    		}
    	}
    	else
    	{
            Buff=40;

            BreathingCycleCount = BreathingCycleCount + 1;
            if (BreathingCycleCount >= Buff)
            {
                BreathingCycleCount = 0;
            }
            TA1CCR1 = BreathingTable_800[BreathingCycleCount];// 20190718  AID
            TA1CCR2 = BreathingTable_800[BreathingCycleCount];
    	}
    }
    else if ( (SystemMode == SystemDischargeMode) || (SystemMode == SystemBootUpWait) )
    {
        if (LED_Mode == Discharge_LED_Mode_1)
            Buff = Breathing_1200;              // 20190719  AID  Breathing_1200
        else
            Buff = Breathing_1600;

        TA1CCR1 = BreathingTable_1600[BreathingCycleCount];
        TA1CCR2 = BreathingTable_1600[BreathingCycleCount];// 20190719  AID

        BreathingCycleCount = BreathingCycleCount + 1;

        if (BreathingCycleCount >= Buff)
        {
            BreathingCycleCount = 0;
        }
    }
    else if(SystemMode == SystemChargeMode)
    {
        if (LED_Mode == Charge_LED_Mode_2)
            Charge_Mode2_LED_Display();
        if (LED_Mode == Charge_LED_Mode_3)
            Charge_Mode3_LED_Display();

        TA1CCR1 = BreathingTable_1600[BreathingCycleCount];
        TA1CCR2 = BreathingTable_1600[BreathingCycleCount];// 20190719  AID
        ChargeLED_Mode2_3_Count = ChargeLED_Mode2_3_Count + 1;
        BreathingCycleCount = BreathingCycleCount + 1;

        if ((LED_Mode == Charge_LED_Mode_1)
                && (BreathingCycleCount >= Breathing_1600))
            BreathingCycleCount = 0;
    }
}

void LED_ALARM(void)
{
    LED_OFF();


    //    BreathingCycleCount = 20;
    //    if(BreathingCycleCount>=79)
    //    {
    //        BreathingCycleCount=0;
    //    }
    //    BreathingCycleCount++;

    //    TA1CCR1 = BreathingTable_1600[BreathingCycleCount];
    //    TA1CCR2 = BreathingTable_1600[BreathingCycleCount];
    //    TA1CCR2 = BreathingCycleCount;
    //    EN_PWM_LID_1_P21;       //ÂÌµÆ
    //    EN_PWM_LID_2_P22;       //ÂÌµÆ
    //    EN_PWM_LID_3_P32;       //ÂÌµÆ
        EN_PWM_LID_4_P32;       //ºìµÆ
        EN_PWM_LID_5_P24;       //LED1Î»ÖÃÀ¶É«
        EN_PWM_LID_6_P25;         //LED2Î»ÖÃÀ¶É«

}

