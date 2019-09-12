//************************************************************
//  Data        :   2019/04/17
//  Programmer  :   Garfield.Lin
//  Function    :   Interrupt
//  MCU         :   MSP430G2553IPW28
//  Oscillator  :   Internal DOC 12MHZ
//  Version     :   V0.1
//
//
//****************************************************************


#include "system.h"


//extern UINT8 SystemMode;

//extern UINT16 SleepCount;

//UINT8 T0_INT_count=0;
//uchar T0_20ms_count=0;
//volatile UINT8 soc_500ms_count =0;
//extern  uchar Alert_count;
extern  UINT8 Sleep_delay;
//extern uchar Alert_count;
//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void TR0A0_ISR(void)
//{
//    _nop ();
//    _nop ();
//}
//#pragma vector=TIMER0_A1_VECTOR
//__interrupt void TR0A1_ISR(void)
//{
//	WDTCTL = WDT_ARST_1000;
//
//    T0_INT_count++;
//
//    Clear_TA0CCTL0_CCIFG;
//    Clear_TA0CCTL1_CCIFG;
//    Clear_TA0CCTL2_CCIFG;
//    TA0CTL&=~TAIFG;
//    soc_500ms_count++;
//
//    key_count();
//
//    if(soc_500ms_count > 25)//25*20 = 500MS
//    {
//        soc_500ms_count=0;
//        Soc_AddSub();//500MS
//    }
//
//    pwm_time_count();           //fenglong 20190712
//
////    if(LPM0_bits)
////    {
////    	LPM0_EXIT;// �˳��͹���ģʽ
////    }
//
//
//}
//#pragma vector=TIMER1_A1_VECTOR
//__interrupt void TR1A1_ISR(void)
//{
//    _nop ();
//    _nop ();
//}
//
//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void TR1A0_ISR(void)
//{
//    _nop ();
//    _nop ();
//}


//#pragma vector=WDT_VECTOR
//
//__interrupt void WatchTimer(void)
//
//{
//    IFG1 &= ~WDTIFG;            //������Ź��жϱ�־
//    _bic_SR_register_on_exit(LPM0_bits);// �˳��͹���ģʽ
//
//}

#pragma vector=PORT1_VECTOR

interrupt void PORT1_ISR (void)
{
//	UART_Initial();
    if(P1IFG&BIT3)
    {

//        if(LPM3_bits==1)
//        {
//        	SystemMode = SystemSleepMode;
//        }

    	_bic_SR_register_on_exit(LPM3_bits);// �˳��͹���ģʽ		fenglong 20190628
        P1IFG &= ~BIT3;     //���IO���жϱ�־
        Sleep_delay=0;

    }
    //���������λ����
    if(P1IFG&BIT4)
    {
//      _bic_SR_register_on_exit(LPM0_bits);// �˳��͹���ģʽ		fenglong 20190628
		_bic_SR_register_on_exit(LPM3_bits);// �˳��͹���ģʽ		fenglong 20190628
        P1IFG &= ~BIT4;     //���IO���жϱ�־
        Sleep_delay=0;
    }
    //ALERT
    if(P1IFG&BIT5)
    {
//      _bic_SR_register_on_exit(LPM0_bits);// �˳��͹���ģʽ		fenglong 20190628
		_bic_SR_register_on_exit(LPM3_bits);// �˳��͹���ģʽ		fenglong 20190628
        P1IFG &= ~BIT5;     //���IO���жϱ�־
        Sleep_delay=0;
//        LED_1_High;
    }
}

#pragma vector=PORT2_VECTOR
interrupt void PORT2_ISR (void)
{

    if(P2IFG&BIT3)			//485
    {
    	UART_Initial();
//      _bic_SR_register_on_exit(LPM0_bits);// �˳��͹���ģʽ		fenglong 20190628
		_bic_SR_register_on_exit(LPM3_bits);// �˳��͹���ģʽ		fenglong 20190628
        P2IFG &= ~BIT3;     //���IO���жϱ�־
        Sleep_delay=0;
//        LED_1_High;
    }
}
