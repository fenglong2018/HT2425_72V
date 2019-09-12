/*
 * uart.h
 *
 *  Created on: 2018年1月24日
 *      Author: zhao
 */

#ifndef UART_H_
#define UART_H_

#include "system.h"




typedef enum
{
     USART_TX_IDLE,
     USART_TX_BUSY,
     USART_TX_ERR,
     USART_TX_OK,

     USART_RX_IDLE,
     USART_RX_BUSY,
     USART_RX_ERR,
     USART_RX_OK,

}USART_STATUS;

typedef struct
{
     uint8_t RxCount;
     uint8_t RxStatus;
     uint32_t RxTime;

     uint8_t TxCount;
     uint8_t TxLength;
     uint8_t TxStatus;

     uint8_t NewReceiveFlag;

     uint8_t SendBit;
}_UsartSet;

extern _UsartSet RS_485;


#define UART_TXIF  (IFG2&UCA0TXIFG)
#define UART_RXIF  (IFG2&UCA0RXIFG)

#define UART_SOI	0X3A
#define UART_EOI	0X7E

#define RX_BUFFER_SIZE 30           //最大只能接收25字节命令
//#define TX_BUFFER_SIZE 30



void Uart_Init();
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);
#define _FILE_INFO_ARG  __FILE__,__FUNCTION__,__LINE__


void UART_Transmit(uchar *txarr,uint length);

void Uart_Init_Debug(void);

#ifndef UART_PRINTF
void Uart_State_Init(void);
#endif


#endif /* UART_H_ */
