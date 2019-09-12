/*
 * balance.c
 *
 *  Created on: 2019年6月14日
 *      Author: fenglong
 */

#include "balance.h"
#include "System.h"

//uint BALANCE_COUNT=0;

extern UINT8 SystemMode;
extern RegisterGroup BQ76930Data;
//extern _FlashData BQ76930ProtectData;
extern _BatteryData BatteryData;
extern UINT16 CellMiniVoltage;
extern UINT16 CellMAXiVoltage;
extern UINT8 TEST_1;
//extern uchar MAXVoltage_Num;



void balance_check(void)
{
    uchar i;
    unsigned short *temp;
    for(i=0;i<7;i++)
    {
        temp=&BatteryData.VCell1.VCell1Word+i;
        if( (*temp - CellMiniVoltage) >= VDIF_100mV )
        {
            switch(i+1)
            {
                case 1:
                    BQ76930Data.CellBal1.CellBal1Bit.CB1=1;
                    break;
                case 2:
                	if( BQ76930Data.CellBal1.CellBal1Bit.CB1 == 0 )
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB2=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB2=0;
                	}
                    break;
                case 3:
                	if( BQ76930Data.CellBal1.CellBal1Bit.CB2 == 0 )
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB3=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB3=0;
                	}
                    break;
                case 4:
                	if( BQ76930Data.CellBal1.CellBal1Bit.CB3 == 0 )
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB5=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal1.CellBal1Bit.CB5=0;
                	}
                    break;

                case 5:
                	if( BQ76930Data.CellBal1.CellBal1Bit.CB5 == 0 )
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB6=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB6=0;
                	}
                    break;
                case 6:
                	if( BQ76930Data.CellBal2.CellBal2Bit.CB6 == 0 )
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB7=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB7=0;
                	}
                    break;
                case 7:
                	if( BQ76930Data.CellBal2.CellBal2Bit.CB7 == 0 )
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB10=1;
                	}
                	else
                	{
                	    BQ76930Data.CellBal2.CellBal2Bit.CB10=0;
                	}
                    break;
                default:
                    break;
            }
        }
        else if(  (*temp - CellMiniVoltage) <= VDIF_40mV )
        {
            switch(i+1)
            {
                case 1:
                    BQ76930Data.CellBal1.CellBal1Bit.CB1=0;
                    break;
                case 2:
                    BQ76930Data.CellBal1.CellBal1Bit.CB2=0;
                    break;
                case 3:
                    BQ76930Data.CellBal1.CellBal1Bit.CB3=0;
                    break;
                case 4:
                    BQ76930Data.CellBal1.CellBal1Bit.CB5=0;
                    break;
                case 5:
                    BQ76930Data.CellBal2.CellBal2Bit.CB6=0;
                    break;
                case 6:
                    BQ76930Data.CellBal2.CellBal2Bit.CB7=0;
                    break;
                case 7:
                    BQ76930Data.CellBal2.CellBal2Bit.CB10=0;
                    break;
                default:
                    break;
            }
        }
    }
}


void exit_balance(void)
{
    if(I2C_Master_TransmitByte(CELLBAL1,0x00) == 0)
    {
        BQ76930Data.CellBal1.CellBal1Byte = 0;
    }
    if(I2C_Master_TransmitByte(CELLBAL2,0x00) == 0)
    {
        BQ76930Data.CellBal2.CellBal2Byte = 0;
    }
}

void balance(void)
{
    if( (SystemMode == SystemChargeMode) && (CellMAXiVoltage>3800) )
    {
        BQ76930Data.CellBal1.CellBal1Byte=0x00;
        BQ76930Data.CellBal2.CellBal2Byte=0x00;
		balance_check();
		//查询需要均衡的串号后开启均衡
		TEST_1=I2C_Master_TransmitByte(CELLBAL1,BQ76930Data.CellBal1.CellBal1Byte);
		TEST_1=I2C_Master_TransmitByte(CELLBAL2,BQ76930Data.CellBal2.CellBal2Byte);
    }
    else
    {
        if( ((BQ76930Data.CellBal1.CellBal1Byte & 0x1F) != 0) || ((BQ76930Data.CellBal2.CellBal2Byte & 0x1F) != 0) )
        {
            exit_balance();
        }
    }
}
