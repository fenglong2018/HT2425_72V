//************************************************************
//  Data        :   2019/04/17
//  Programmer  :   Garfield.Lin
//  Function    :   I2C
//  MCU         :   MSP430G2553IPW28
//  Oscillator  :   Internal DOC 12MHZ
//  Version     :   V0.1
//
//
//****************************************************************

#include "system.h"

unsigned char IIC_Buffer[22];

unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
{
    unsigned char i;
    unsigned char crc=0;
    while(len--!=0)
    {
        for(i=0x80; i!=0; i/=2)
        {
            if((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= key;
            }
            else
                crc *= 2;

            if((*ptr & i)!=0)
                crc ^= key;
        }
        ptr++;
    }
    return(crc);
}

int I2C_Load_BQ76930 (UINT8 *DataBuffer ,UINT8 Addr, UINT8 Length)
{
    unsigned char CRCInput[2];
    unsigned char CRC = 0;
    int i;

    I2CReadBytes(Addr, IIC_Buffer, 2 * Length);

    CRCInput[0] = ADDR_IIC_R;
    CRCInput[1] = IIC_Buffer[0];

    CRC = CRC8(CRCInput, 2, CRC_KEY);

    if (CRC != IIC_Buffer[1])
    {
        return 1;
    }
    else
    {
        *DataBuffer = IIC_Buffer[0];
    }


    DataBuffer++;
    if(Length>1)
    {
        for(i = 1; i < Length ; i++)
        {
            CRCInput[0] = IIC_Buffer[i*2];
            CRC = CRC8(CRCInput, 1, CRC_KEY);

            if (CRC != IIC_Buffer[i*2+1])
            {
                return 1;
            }
            else
            {
                *DataBuffer = IIC_Buffer[i*2];
            }
            DataBuffer++;
        }
    }
    return 0;
}

int I2C_Master_TransmitByte (UINT8 Addr, UINT8 Data)
{
    IIC_Buffer[0] = ADDR_IIC_W;
    IIC_Buffer[1] = Addr;
    IIC_Buffer[2] = Data;
    IIC_Buffer[3] = CRC8(IIC_Buffer, 3, CRC_KEY);
    //Buffer++;
//    if(Length>1)
//    {
//        for(i = 1; i < Length; i++)
//        {
//            data[0] = *Buffer;
//            IIC_Buffer[i*2+2] = *Buffer;
//            IIC_Buffer[i*2+3] = CRC8(data, 1, CRC_KEY);
//            Buffer++;
//        }
//    }
    I2CSendBytes(IIC_Buffer, (2*1 + 2));
    return 0;
}

void I2CSendBytes(unsigned char *DataBuffer, unsigned int ByteCount)
{
    unsigned char i = 0;
    IIC_Start();
    for(i=0;i<ByteCount;i++)
    {
      IIC_Send_Byte(*DataBuffer);
      DataBuffer++;
    }
    IIC_Stop();
}

int I2CReadBytes(unsigned char targer, unsigned char *DataBuffer, unsigned int ExpectedByteNumber)
{
    u8 i=0;

    IIC_Start();
    IIC_Send_Byte(ADDR_IIC_W);
    IIC_Send_Byte(targer);
    delay_us(20);
    IIC_Start();
    IIC_Send_Byte(ADDR_IIC_R);
    for(i=0;i<ExpectedByteNumber-1;i++)
    {
       *DataBuffer =IIC_Read_Byte(ACK);
       DataBuffer++;
    }
    *DataBuffer =IIC_Read_Byte(NACK);
    IIC_Stop();
    return 0;
}




