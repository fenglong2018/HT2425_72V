


#include "system.h"
//#include"uart.h"

extern uchar CRC_DATA;
extern  UINT8 Sleep_delay;
//extern uchar Alert_count;


//	static uint8_t usartTxHead = 0;
//	static uint8_t usartTxTail = 0;
//
//	static uint8_t RxHead = 0;
//	static uint8_t RxTail = 0;
//
//	static uint8_t eusartTxBufferRemaining = 0;
//	static uint8_t eusartusartRxbufferRemaining = 0;
	
//	volatile uint8_t TxCount;
//	volatile uint8_t RxCount;
	
//	uint8_t usartTxBuffer[TX_BUFFER_SIZE];
volatile    uchar usartRxbuffer[RX_BUFFER_SIZE];

	_UsartSet RS_485;



//---------------------------------------------//




//---------------------------------------------//




    void Uart_State_Init(void)
    {
        RS_485.RxCount = 0;
        RS_485.RxStatus = USART_RX_IDLE;
        RS_485.TxCount = 0;
        RS_485.TxLength = 0;
        RS_485.TxStatus = USART_TX_IDLE;

        RS_485.NewReceiveFlag = 0;

    }



//---------------------------------------------//



	

	
//	void Uart_Init() //SMCLK
//	{
//		UCA0CTL1 = UCSWRST;
//	    P1DIR  |= BIT2;             //P1.2  UART_TX
//	    P1DIR  &=~BIT1;             //P1.1  UART_RX
//	    P1SEL  |= BIT1+BIT2;        //select P1.1 and P1.2 as UART port
//	    P1SEL2 |= BIT1+BIT2;
//
//		UCA0CTL1 |= UCSSEL_2;                     // SMCLK ,8N1
//		UCA0BR0 = 138;                            // 8M  57600
//		UCA0BR1 = 0x00;                              //
//		UCA0MCTL = 4;              //
//		UCA0CTL1 &= ~UCSWRST;      //闂佽崵濮崇粈浣规櫠娴犲鍋柛鈩冪☉閻鏌涚仦鍓р檨婵炲牆鐖奸弻锝夊閵忕姵鐎鹃梺鍝勬嫅閹凤拷
//	    IE2 |= UCA0RXIE ;  //Open the UART recievc interrupt
//
//	    Uart_State_Init();
//	}
	
	
	
	#pragma vector=USCIAB0RX_VECTOR

	__interrupt void USCI0RX_ISR(void)
	{
//      _bic_SR_register_on_exit(LPM0_bits);// 退出低功耗模式		fenglong 20190628
		_bic_SR_register_on_exit(LPM3_bits);// 退出低功耗模式		fenglong 20190628
		Sleep_delay=0;
	    //Byte_Recv  = UCA0RXBUF;      //load the recieve byte
	    IFG2 &=~UCA0RXIFG;         //clear RX interrupt flag

	    usartRxbuffer[RS_485.RxCount] = UCA0RXBUF;

		if (usartRxbuffer[0]!=UART_SOI)
		{
			RS_485.RxCount=0;
			RS_485.RxStatus = USART_RX_IDLE;
		}
		else
		{
            if(usartRxbuffer[RS_485.RxCount]==UART_EOI)         //RECEIVE END
            {
                RS_485.RxStatus = USART_RX_OK;
                RS_485.RxCount++;
            }
            else
            {
                RS_485.RxCount++;
                RS_485.RxStatus=USART_RX_BUSY;
            }
		}

		if(RS_485.RxCount>=RX_BUFFER_SIZE-2)
		{
		    RS_485.RxCount=0;
		    usartRxbuffer[0]=0;
		    RS_485.RxStatus = USART_RX_IDLE;
		}
//		_bic_SR_register_on_exit(LPM3_bits);// 退出低功耗模式
	}

void UART_Transmit(uchar *txarr,uint length)
{
    uchar  temp ;
    uint i;

    for(i=0;i<length;i++)
    {

        while (!(IFG2&UCA0TXIFG));
        temp=(txarr[i] & 0xF0)>>4;

        if(temp>9)
        {
            UCA0TXBUF = temp + 0x37;
            CRC_DATA += temp + 0x37;
        }
        else
        {
            UCA0TXBUF = temp + 0x30;
            CRC_DATA += temp + 0x30;
        }



        while (!(IFG2&UCA0TXIFG));

        temp=txarr[i] & 0x0F;

        if(temp>9)
        {
            UCA0TXBUF = temp + 0x37;
            CRC_DATA += temp + 0x37;
        }
        else
        {
            UCA0TXBUF = temp + 0x30;
            CRC_DATA += temp + 0x30;
        }
        while (!(IFG2&UCA0TXIFG));
    }


}
	



	



