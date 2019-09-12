/*********************************************************************************************************//**
 * @file    i2c_eeprom.c
 * @version $Rev:: 1736         $
 * @date    $Date:: 2017-08-25 #$
 * @brief   The source file of i2c_eeprom.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "drv_USI_IIC.h"
#include <msp430.h>

/*
 * File:   IO_I2C.c
 * Author: AID
 *
 * ??IIC????
 *
 * Created on 2016?9?22?, ??5:46
 */
void delay_us(uchar g_us)
{
    uchar i;
    for(i=0;i<g_us;i++)
    {
        _delay_cycles(12);
    }
}
/****************************************************************************
FUNCTION        : IIC_Init
DESCRIPTION     : ??IIC???
INPUT           : None
OUTPUT          : None
NOTICE          : ??IO????????,???????????????
DATE            : 2016/09/21
*****************************************************************************/
void IIC_Init(void)
{
    IIC_SDA_OUT();
    IIC_SCL_OUT();
    IIC_SDA_L();
    IIC_SCL_L();
    delay_us(200);
    IIC_SCL_H();
    IIC_SDA_H();
}

/****************************************************************************
FUNCTION        : IIC_Start
DESCRIPTION     : ??IIC??IIC????
INPUT           : None
OUTPUT          : None
NOTICE          :
DATE            : 2016/09/21
*****************************************************************************/
void IIC_Start(void)
{
    IIC_SDA_OUT();//SDA??
    IIC_SCL_H();
    IIC_SDA_H();
    delay_us(10);
    IIC_SDA_L();//START:when CLK is high,DATA change form high to low
    delay_us(10);
    IIC_SCL_L();//??CLK??,??????
}
/****************************************************************************
FUNCTION        : IIC_Stop
DESCRIPTION     : ??IIC??IIC????
INPUT           : None
OUTPUT          : None
NOTICE          :
DATE            : 2016/09/21
*****************************************************************************/
void IIC_Stop(void)
{
    IIC_SDA_OUT();//SDA??
    IIC_SCL_L();
    IIC_SDA_L();//STOP:when CLK is high DATA change form low to high
    delay_us(10);
    IIC_SCL_H();
    IIC_SDA_H();
    delay_us(10);
}
/****************************************************************************
FUNCTION        : IIC_Wait_Ack
DESCRIPTION     : ??IIC??ACK??
INPUT           : None
OUTPUT          : None
NOTICE          : ????1??,0??
DATE            : 2016/09/21
*****************************************************************************/
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    IIC_SDA_H();
  IIC_SDA_IN();      //SDA???????
    IIC_SCL_H();
    delay_us(5);
    while(IIC_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L();
    return 0;
}

/****************************************************************************
FUNCTION        : IIC_Ack
DESCRIPTION     : ??IIC??ACK????
INPUT           : None
OUTPUT          : None
NOTICE          :
DATE            : 2016/09/21
*****************************************************************************/
void IIC_Ack(void)
{
    IIC_SCL_L();
    IIC_SDA_OUT();//SDA??
    IIC_SDA_L();//
    delay_us(5);
    IIC_SCL_H();
    delay_us(5);
    IIC_SCL_L();
}
/****************************************************************************
FUNCTION        : IIC_NAck
DESCRIPTION     : ??IIC??NACK????
INPUT           : None
OUTPUT          : None
NOTICE          :
DATE            : 2016/09/21
*****************************************************************************/
void IIC_NAck(void)
{
    IIC_SCL_L();
    IIC_SDA_OUT();//SDA??
    IIC_SDA_H();
    delay_us(5);
    IIC_SCL_H();
    delay_us(5);
    IIC_SCL_L();
}
/****************************************************************************
FUNCTION        : IIC_Send_Byte
DESCRIPTION     : ??IIC??????
INPUT           : None
OUTPUT          : None
NOTICE          :
DATE            : 2016/09/21
*****************************************************************************/
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    IIC_SDA_OUT();//SDA??
    IIC_SCL_L();
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>>7)
        {
            IIC_SDA_H();
        }
        else
        {
            IIC_SDA_L();
        }
        txd<<=1;
        delay_us(5);
        IIC_SCL_H();
        delay_us(5);
        IIC_SCL_L();
    }
    IIC_Wait_Ack();
}
/****************************************************************************
FUNCTION        : IIC_Read_Byte
DESCRIPTION     : ??IIC??????
INPUT           : None
OUTPUT          : None
NOTICE          : ??1???ACK?1??ACK,?0??NACK
DATE            : 2016/09/21
*****************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    IIC_SDA_IN();//SDA??
    IIC_SCL_L();
    delay_us(5);
    for(i=0;i<8;i++ )
    {
        IIC_SCL_H();
        delay_us(5);
        receive<<=1;
        if(IIC_READ_SDA)
        {
            receive++;
        }
        IIC_SCL_L();
        delay_us(5);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}




