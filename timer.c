/*
 * timer.c
 *
 *  Created on: 2019Äê5ÔÂ16ÈÕ
 *      Author: Administrator
 */
#include "timer.h"
#include "system.h"

extern union _LED_CTRL LED_CTRL;
extern volatile uchar led1_set_rate;
extern volatile uchar led2_set_rate;
extern volatile uchar led3_set_rate;
UINT8 T0_INT_count=0;
uchar T0_20ms_count=0;
uchar T0_200ms_count=0;
uchar T0_1s_count=0;
uchar T0_1m_count=0;

uchar T0_20ms_old=0;
uchar T0_200ms_old=1;
uchar T0_1s_old=1;
uchar T0_1m_old=1;

//volatile UINT8 soc_500ms_count =0;


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
//    LED_4_Low;
//----------------------------------//
    LED1_PWM(BIT1);
    LED2_PWM(BIT2);
    LED3_PWM(BIT3);
//----------------------------------//
//    LED_4_High;
}


void timer1_init(void)
{
    TA1CTL = TASSEL_2 + MC_1 + ID_3;                  // SMCLK, contmode
    TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA1CCR0 = 187;     //0.125ms
    //  TACCTL1 = CCIE;                             // CCR0 interrupt enabled
    TA1CCR1 = 30000;
    TA1CCR2 = 30000;
}

void close_timer1(void)
{
	if( (TA1CCTL0 & CCIE) == CCIE)
	{
		TA1CCTL0 &= ~CCIE ;
	    TA1CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;            //Timer 1 , SMCLK / 1 , TA1CCTL0 , PWM
	    TA1CCTL0 = OUTMOD_6;
	    TA1CCTL1 = OUTMOD_6;
	    TA1CCTL2 = OUTMOD_6;

	    TA1CCR0 = 0xFFFF;
	    TA1CCR1 = 0;
	    TA1CCR2 = 0;

	    Clear_TA1CCTL0_CCIFG;
	    Clear_TA1CCTL1_CCIFG;
	    Clear_TA1CCTL2_CCIFG;
	}

//    TA1CCTL0 &= ~CCIE ;
//    TA1CCR0 = 0 ;
//    TA1CCR1 = 0 ;
//    TA1CCR2 = 0 ;
//    Timer_A_initial();
}

void pwm_time_count(void)
{
    if( LED_CTRL.Bits.PWM1_UP == 1 )
    {
        led1_set_rate+=7;

        if(led1_set_rate>95)
        {
            led1_set_rate=100;
        }
    }
    else
    {
        if(led1_set_rate>=7)
        {
            led1_set_rate-=7;
        }
        else
        {
            led1_set_rate=0;
        }
    }
    //-----------------------------------//
    if( LED_CTRL.Bits.PWM2_UP == 1 )
    {
        led2_set_rate+=7;

        if(led2_set_rate>95)
        {
            led2_set_rate=100;
        }
    }
    else
    {
        if(led2_set_rate>=7)
        {
            led2_set_rate-=7;
        }
        else
        {
            led2_set_rate=0;
        }
    }

    //---------------------------------//

    if( LED_CTRL.Bits.PWM3_UP == 1 )
    {
        led3_set_rate+=7;

        if(led3_set_rate>95)
        {
            led3_set_rate=100;
        }
    }
    else
    {
        if(led3_set_rate>=7)
        {
            led3_set_rate-=7;
        }
        else
        {
            led3_set_rate=0;
        }
    }
}



#pragma vector=TIMER0_A1_VECTOR
__interrupt void TR0A1_ISR(void)
{
    Clear_TA0CCTL0_CCIFG;
    Clear_TA0CCTL1_CCIFG;
    Clear_TA0CCTL2_CCIFG;
    TA0CTL&=~TAIFG;
    T0_20ms_count++;
    if(T0_20ms_count>=10)
    {
    	WDTCTL = WDT_ARST_1000;
    	T0_20ms_count=0;
    	T0_200ms_count++;
    	if(T0_200ms_count>=5)
    	{
    		T0_200ms_count=0;
    		T0_1s_count++;
    		if(T0_1s_count>=60)
    		{
    			T0_1s_count=0;
    			T0_1m_count++;
    			if(T0_1m_count>=200)
    			{
    				T0_1m_count=0;
    			}
    		}
    	}
    }
    pwm_time_count();           //fenglong 20190712

    LPM0_EXIT;

}





