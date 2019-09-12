/*
 * I2C.H
 *
 *  Created on: 2019¦~4¤ë12¤é
 *      Author: ET
 */


#ifndef _I2C_H_
#define _I2C_H_


#define BQMAXIMO 0x10
#define ADDR_IIC_W 0x10
#define ADDR_IIC_R 0x11

int I2C_Load_BQ76930 (UINT8 *DataBuffer ,UINT8 Addr, UINT8 Length);

int I2C_Master_TransmitByte (UINT8 Addr, UINT8 Data);
int I2CReadBytes(unsigned char targer, unsigned char *DataBuffer, unsigned int ExpectedByteNumber);
void I2CSendBytes(unsigned char *DataBuffer, unsigned int ByteCount);
//int I2C_Master_TransmitData (UINT8 Addr, UINT8 *Data, UINT8 Length);
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key);

#endif

