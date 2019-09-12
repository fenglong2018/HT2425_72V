/*
 * bqMaximo_Ctrl_G2553.h
 *
 * bq769x0 CRC code example header file
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


/*
#define ADCGAIN_BASE			365

#define LOW_BYTE(Data)			(unsigned char)(0xff & Data)
#define HIGH_BYTE(Data)			(unsigned char)(0xff & (Data >> 8))



unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key);

int I2CWriteRegisterByteWithCRC(unsigned char I2CSlaveAddress, unsigned char Register, unsigned char Data);

int I2CReadRegisterWordWithCRC(unsigned char I2CSlaveAddress, unsigned char Register, unsigned int *Data);
*/
typedef struct _Register_Group
{
	union
	{
		struct
		{
			unsigned char OCD			:1;
			unsigned char SCD			:1;
			unsigned char OV			:1;
			unsigned char UV			:1;
			unsigned char OVRD_ALERT	:1;
			unsigned char DEVICE_XREADY	:1;
			unsigned char WAKE			:1;
			unsigned char CC_READY		:1;
		}StatusBit;
		unsigned char StatusByte;
	}SysStatus;

	union
	{
		struct
		{
		    unsigned char CB1           :1;
		    unsigned char CB2           :1;
		    unsigned char CB3           :1;
		    unsigned char CB4           :1;
		    unsigned char CB5           :1;
			unsigned char RSVD			:3;
		}CellBal1Bit;
		unsigned char CellBal1Byte;
	}CellBal1;

	union
	{
		struct
		{
		    unsigned char CB6           :1;
		    unsigned char CB7           :1;
		    unsigned char CB8           :1;
		    unsigned char CB9           :1;
		    unsigned char CB10          :1;
			unsigned char RSVD			:3;
		}CellBal2Bit;
		unsigned char CellBal2Byte;
	}CellBal2;

//	union
//	{
//		struct
//		{
//		    unsigned char CB11          :1;
//		    unsigned char CB12          :1;
//		    unsigned char CB13          :1;
//		    unsigned char CB14          :1;
//		    unsigned char CB15          :1;
//			unsigned char RSVD			:3;
//		}CellBal3Bit;
//		unsigned char CellBal3Byte;
//	}CellBal3;

	union
	{
		struct
		{
			unsigned char SHUT_B		:1;
			unsigned char SHUT_A		:1;
			unsigned char RSVD1			:1;
			unsigned char TEMP_SEL		:1;
			unsigned char ADC_EN		:1;
			unsigned char RSVD2			:2;
			unsigned char LOAD_PRESENT	:1;
		}SysCtrl1Bit;
		unsigned char SysCtrl1Byte;
	}SysCtrl1;

	union
	{
		struct
		{
			unsigned char CHG_ON		:1;
			unsigned char DSG_ON		:1;
			unsigned char WAKE_T		:2;
			unsigned char WAKE_EN		:1;
			unsigned char CC_ONESHOT	:1;
			unsigned char CC_EN			:1;
			unsigned char DELAY_DIS		:1;
		}SysCtrl2Bit;
		unsigned char SysCtrl2Byte;
	}SysCtrl2;

	union
	{
		struct
		{
			unsigned char SCD_THRESH	:3;
			unsigned char SCD_DELAY		:2;
			unsigned char RSVD			:2;
			unsigned char RSNS			:1;
		}Protect1Bit;
		unsigned char Protect1Byte;
	}Protect1;

	union
	{
		struct
		{
			unsigned char OCD_THRESH	:4;
			unsigned char OCD_DELAY		:3;
			unsigned char RSVD			:1;
		}Protect2Bit;
		unsigned char Protect2Byte;
	}Protect2;

	union
	{
		struct
		{
			unsigned char RSVD			:4;
			unsigned char OV_DELAY		:2;
			unsigned char UV_DELAY		:2;
		}Protect3Bit;
		unsigned char Protect3Byte;
	}Protect3;

	unsigned char OVTrip;
	unsigned char UVTrip;
	unsigned char CCCfg;			//must be 0x19

	union
	{
		struct
		{
			unsigned char VC1_HI;
			unsigned char VC1_LO;
		}VCell1Byte;
		unsigned short VCell1Word;
	}VCell1;

	union
	{
		struct
		{
			unsigned char VC2_HI;
			unsigned char VC2_LO;
		}VCell2Byte;
		unsigned short VCell2Word;
	}VCell2;

	union
	{
		struct
		{
			unsigned char VC3_HI;
			unsigned char VC3_LO;
		}VCell3Byte;
		unsigned short VCell3Word;
	}VCell3;

	union
	{
		struct
		{
			unsigned char VC4_HI;
			unsigned char VC4_LO;
		}VCell4Byte;
		unsigned short VCell4Word;
	}VCell4;

	union
	{
		struct
		{
			unsigned char VC5_HI;
			unsigned char VC5_LO;
		}VCell5Byte;
		unsigned short VCell5Word;
	}VCell5;

	union
	{
		struct
		{
			unsigned char VC6_HI;
			unsigned char VC6_LO;
		}VCell6Byte;
		unsigned short VCell6Word;
	}VCell6;

	union
	{
		struct
		{
			unsigned char VC7_HI;
			unsigned char VC7_LO;
		}VCell7Byte;
		unsigned short VCell7Word;
	}VCell7;

	union
	{
		struct
		{
			unsigned char VC8_HI;
			unsigned char VC8_LO;
		}VCell8Byte;
		unsigned short VCell8Word;
	}VCell8;

	union
	{
		struct
		{
			unsigned char VC9_HI;
			unsigned char VC9_LO;
		}VCell9Byte;
		unsigned short VCell9Word;
	}VCell9;

	union
	{
		struct
		{
			unsigned char VC10_HI;
			unsigned char VC10_LO;
		}VCell10Byte;
		unsigned short VCell10Word;
	}VCell10;

//	union
//	{
//		struct
//		{
//			unsigned char VC11_HI;
//			unsigned char VC11_LO;
//		}VCell11Byte;
//		unsigned short VCell11Word;
//	}VCell11;
//
//	union
//	{
//		struct
//		{
//			unsigned char VC12_HI;
//			unsigned char VC12_LO;
//		}VCell12Byte;
//		unsigned short VCell12Word;
//	}VCell12;
//
//	union
//	{
//		struct
//		{
//			unsigned char VC13_HI;
//			unsigned char VC13_LO;
//		}VCell13Byte;
//		unsigned short VCell13Word;
//	}VCell13;
//
//	union
//	{
//		struct
//		{
//			unsigned char VC14_HI;
//			unsigned char VC14_LO;
//		}VCell14Byte;
//		unsigned short VCell14Word;
//	}VCell14;
//
//	union
//	{
//		struct
//		{
//			unsigned char VC15_HI;
//			unsigned char VC15_LO;
//		}VCell15Byte;
//		unsigned short VCell15Word;
//	}VCell15;

	union
	{
		struct
		{
			unsigned char BAT_HI;
			unsigned char BAT_LO;
		}VBatByte;
		unsigned short VBatWord;
	}VBat;

	union
	{
		struct
		{
			unsigned char TS1_HI;
			unsigned char TS1_LO;
		}TS1Byte;
		unsigned short TS1Word;
	}TS1;

	union
	{
		struct
		{
			unsigned char TS2_HI;
			unsigned char TS2_LO;
		}TS2Byte;
		unsigned short TS2Word;
	}TS2;

	union
	{
		struct
		{
			unsigned char TS3_HI;
			unsigned char TS3_LO;
		}TS3Byte;
		unsigned short TS3Word;
	}TS3;

	union
	{
		struct
		{
			unsigned char CC_HI;
			unsigned char CC_LO;
		}CCByte;
		unsigned short CCWord;
	}CC;

	union
	{
		struct
		{
			unsigned char RSVD1			:2;
			unsigned char ADCGAIN_4_3	:2;
			unsigned char RSVD2			:4;
		}ADCGain1Bit;
		unsigned char ADCGain1Byte;
	}ADCGain1;

	unsigned char ADCOffset;

	union
	{
		struct
		{
			unsigned char RSVD			:5;
			unsigned char ADCGAIN_2_0	:3;
		}ADCGain2Bit;
		unsigned char ADCGain2Byte;
	}ADCGain2;

} RegisterGroup;

