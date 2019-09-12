/*
 * drv_USI_IIC.h
 *
 *  Created on: 2013-8-12
 *      Author: hougongyan
 */

#ifndef DRV_USI_IIC_H_
#define DRV_USI_IIC_H_


#include <msp430.h>
#include "system.h"

#define ACK  1
#define NACK 0
#define IICREAD 1
#define IICWRITE 0
#define CRC_KEY 7
//-----------------------------------------Description--------------------------------------------//
/*
* pin map
* SCL           -- P3.6
* SDA           -- P3.7
*/

//------------------------------------Add your macro----------------------------------------------//

/*change SDA direction to OUT*/
#define IIC_SDA_OUT()        {P1DIR |= BIT7;}

/*change SDA dirction to IN*/
#define IIC_SDA_IN()         {P1DIR &= (~BIT7);}
#define IIC_SCL_OUT()        {P1DIR |= BIT6;}

/*pin operate define */
#define IIC_SCL_H()           (P1OUT |= BIT6)
#define IIC_SCL_L()           (P1OUT &= (~BIT6))

#define IIC_SDA_H()           (P1OUT |= BIT7)
#define IIC_SDA_L()           (P1OUT &= (~BIT7))


#define I2C_OK                  0U
#define I2C_RECEIVE_ACK_ERR     1U

//------------------------------------Add your typedef----------------------------------------------//

//------------------------------------Add your value----------------------------------------------//
/*SDA input data*/
#define IIC_READ_SDA         ((P1IN & BIT7)>>7)  //(GpioDataRegs.GPBDAT.bit.GPIO50)


//-----------------函数声明----------------------------

//------------------------------------Add your function declare-----------------------------------//
//api
void IIC_Init(void);                //IIC初始化
void IIC_Start(void);                               //IIC开始信号
void IIC_Stop(void);                            //IIC停止信号
void IIC_Send_Byte(uchar txd);                 //IIC发送1Byte数据
uchar IIC_Read_Byte(unsigned char ack);//IIC读取1Byte数据
uchar IIC_Wait_Ack(void);                          //WAIT_ACK
void IIC_Ack(void);                                 //SEND_ACK
void IIC_NAck(void);                                //NACK
uchar IICStartFlag(uchar addr, uchar rw);        //构建首帧数据
uchar IIC_Send_Str(uchar * data, uchar lenght);
uchar IIC_Read_Str(uchar * data, uchar lenght);
void delay_us(uchar g_us);



#endif /* DRV_USI_IIC_H_ */
