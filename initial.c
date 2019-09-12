//************************************************************
//  Data        :   2019/04/17
//  Programmer  :   Garfield.Lin
//  Function    :   System initial
//  MCU         :   MSP430G2553IPW28
//  Oscillator  :   Internal DOC 12MHZ
//  Version     :   V0.1
//
//
//****************************************************************


#include "System.h"
#include "Initial.h"
#include "uart.h"

extern union FW_PT FW_PT_Status;
extern union SW_ALARM ALARM_PT_Status;        //警告值状态
extern _BatteryData BatteryData;
extern unsigned char  power_first_flag;
UINT16 his_data[FLASH_DATA_NUM];
extern UINT8 BatteryCapacityRefreshCount;
extern union FW_PF FW_PF_Status;        //  三级保护？
extern UINT8 SystemMode;
extern RegisterGroup BQ76930Data;
extern signed short iGain;


void    Calibration (void)
{
    DCOCTL = 0x60;
    BCSCTL1 = BCSCTL1 | 0x0F;
    BCSCTL2 = 0x00;                          //MCLK = DCOCLK,(1:1), SMCLK = DCOCLK,(1:1)
    BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO

    if (CALBC1_12MHZ==0xFF)                    // If calibration constant erased
    {
        while(1)                               // If calibration constants erased                                          // do not load, trap CPU!!
        {
            asm("  MOV &0xFFFE,PC;");//将PC转移到应用程序的复位向量处..调用汇编的时候记得前面有空格！！！              //fenglong 20190615
        }

    }

    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_12MHZ;                    // Set DCO to 12MHz
    DCOCTL =  CALDCO_12MHZ;

}

void initClockTo12MHz()
{
    if (CALBC1_12MHZ==0xFF)                  // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_12MHZ;                    // Set DCO
    DCOCTL = CALDCO_12MHZ;
}



void GPIOInitial(void)
{
    EN_LED_1_P21;
    EN_LED_2_P22;
    EN_LED_3_P33;
    EN_LED_4_P32;
    EN_LED_5_P24;
    EN_LED_6_P25;

    EN_SW_pin;
    EN_DC_IN_pin;
    EN_76930_Alert_Pin;
    EN_WakeUP_Pin;

    EN_RE_485_Pin;
    EN_CP_Pin;
    EN_DE_485_Pin;
    EN_MCU_SAFE_Pin;

    LED_OFF();

    CP_High;
    EN_RT_ON_Pin;
    RT_ON_HIGH;

    EN_ALERT_Pin;
    EN_DC_Pin;

}

void UART_Initial (void)
{
    EN_UART_RX_P11;
    EN_UART_TX_P12;

    RS485_RE;

    UCA0CTL0 = 0x00;
    UCA0CTL1 = UCA0CTL1 | UCSSEL1;
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    UCA0BR0 = 78;                             // 12MHz 9600
    UCA0BR1 = 0;                              // 12MHz 9600

    UCA0MCTL = UCBRS_0 + UCOS16 + UCBRF_2;    // Modulation UCBRSx = 0, UCBRSx = 2, UCOS16 = 1;

    IFG2 &= ~(UCA0RXIFG);
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
//    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
}


void I2C_Initial(void)
{
    IIC_Init();
    /*
    EN_P16_OUT;
    EN_P17_OUT;
    _nop ();
    //EN_I2C_SCL_P16;
    //EN_I2C_SDA_P17;

    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // Master, I2C Mode,synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
//    UCB0BR0 = 120;                             // fSCL = SMCLK/120 = ~100kHz
    UCB0BR0 = 140;                             // fSCL = SMCLK/120 = 80kHz
    UCB0BR1 = 0;
    UCB0I2CSA = BQ76930_Address;                         // Set slave address
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//    IE2 |= UCB0RXIE;                          // Enable RX interrupt
 //   TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
  * */

}
void Timer_A_initial(void)
{
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR + TAIE;     //Timer 0 , SMCLK / 8 , TA0CCTL0 , INT

    TA0CCR0 = TA0_20ms;
    TA0CCR1 = 0xF000;
    TA0CCR2 = 0xF000;

    Clear_TA0CCTL0_CCIFG;
    Clear_TA0CCTL1_CCIFG;
    Clear_TA0CCTL2_CCIFG;


    TA1CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;            //Timer 1 , SMCLK / 1 , TA1CCTL0 , PWM
    TA1CCTL0 = OUTMOD_6;
    TA1CCTL1 = OUTMOD_6;
    TA1CCTL2 = OUTMOD_6;

    TA1CCR0 = 0xFFFF;
   // TA1CCR1 = 0x2000;
    //TA1CCR2 = 0x2000;

    Clear_TA1CCTL0_CCIFG;
    Clear_TA1CCTL1_CCIFG;
    Clear_TA1CCTL2_CCIFG;
}

void ConfigWDT(void)
 {
//  WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    WDTCTL = WDT_ARST_1000;             //AID 20190717
    //IFG1 &= ~WDTIFG;            //清除看门狗中断标志
    //IE1 |= WDTIE;               // 看门狗中断使能10/23xxy


 }

void RAM_Init(void)
{

	his_data_init();

    ALARM_PT_Status.Byte = 0 ;
    BatteryData.CC.CCWord =0;
    BatteryData.VBat.VBatWord =22000;
    BatteryData.TS1.TS1Word =3000;
    BatteryData.TS2.TS2Word =3000;
    BatteryData.VCell1.VCell1Word =3600;
    BatteryData.VCell2.VCell2Word =3600;
    BatteryData.VCell3.VCell3Word =3600;
    BatteryData.VCell4.VCell4Word =3600;
    BatteryData.VCell5.VCell5Word =3600;
    BatteryData.VCell6.VCell6Word =3600;
    BatteryData.VCell7.VCell7Word =3600;

    FW_PT_Status.Word=0;        //fenglong 20190602

	if(sys_cap.val.bat_cycle_cnt < 1000)
	{
	    sys_cap.val.full_cap = (2500-sys_cap.val.bat_cycle_cnt);
	}


	power_first_flag=0;			//fenglong 20190713
	BatteryCapacityRefreshCount = 0 ;		//fenglong 20190828

	SystemMode = SystemSleepMode;
//	SystemMode = SystemDischargeMode;
}

void BlackBox_Init(void)
{
	uchar i;
	unsigned short crc16;

    for(i =0;i<FLASH_DATA_NUM;i++)
    {
    	his_data[i] = Flash_Read_int((unsigned int *)Segment_C,i);
    }
    crc16 =0;
    for(i=0;i<FLASH_DATA_NUM-1;i++)
    {
        crc16 += his_data[i];
    }
    if((crc16 == his_data[HIS_CRC16])&&(his_data[0] == FLASH_CHECK))
    {
        FW_PF_Status.Word = his_data[PF_STATE_ADDR];
        if(his_data[HIS_DSG_UVP_ADD])
        {
            FW_PT_Status.Bits.PT_UV = 1; // 临时屏蔽
        }
        sys_cap.val.bat_cycle_cnt = his_data[FLASH_CYCLES_ADDR];              //fenglong 20190615
        sys_cap.val.chg_cap_cnt = (uchar)(his_data[CHG_CAP_CNT]) ;
    }
	else
	{
		for(i =0;i<FLASH_DATA_NUM;i++)
		{
			his_data[i] = Flash_Read_int((unsigned int *)Segment_D,i);
		}
		crc16 =0;
        for(i=0;i<FLASH_DATA_NUM-1;i++)
        {
            crc16 += his_data[i];
        }
		if((his_data[0] == FLASH_CHECK)&&(crc16 == his_data[HIS_CRC16]))
		{
			FW_PF_Status.Word = his_data[PF_STATE_ADDR];
			if(his_data[HIS_DSG_UVP_ADD])
            {
                FW_PT_Status.Bits.PT_UV = 1;// 临时屏蔽
            }
			sys_cap.val.bat_cycle_cnt=his_data[FLASH_CYCLES_ADDR];                      //fenglong 20190615
			sys_cap.val.chg_cap_cnt = (uchar)(his_data[CHG_CAP_CNT]) ;
			Falsh_Write_Arr((unsigned int *)Segment_C,his_data,FLASH_DATA_NUM);         //fenglong 20190615
			//
		}
		else
		{
		    crc16 =0;
			his_data[0] = FLASH_CHECK;
			crc16 += his_data[0];
			for(i=1;i<FLASH_DATA_NUM-1;i++)
			{
				his_data[i] =0;
			}
			his_data[HIS_CRC16] = crc16;
			Falsh_Write_Arr((unsigned int *)Segment_C,his_data,FLASH_DATA_NUM);         //fenglong 20190615
			Falsh_Write_Arr((unsigned int *)Segment_D,his_data,FLASH_DATA_NUM);         //fenglong 20190615
			FW_PF_Status.Word =0;
			FW_PT_Status.Bits.PT_UV =0;
			sys_cap.val.bat_cycle_cnt=0;                                                //fenglong 20190615
			sys_cap.val.chg_cap_cnt = 0;

			his_data_init();


		}
	}
}

void his_data_init(void)
{
	his_data[HIS_Cell1MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell2MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell3MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell4MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell5MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell6MiniVoltage]=4200;                                        //fenglong 20190617
	his_data[HIS_Cell7MiniVoltage]=4200;                                        //fenglong 20190617

	his_data[HIS_Cell1MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell2MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell3MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell4MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell5MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell6MAXiVoltage]=0;                                        //fenglong 20190730
	his_data[HIS_Cell7MAXiVoltage]=0;                                        //fenglong 20190730

	his_data[HIS_MINVOLT] = 0xffff;
	his_data[HIS_MAXVOLT] = 0;

	his_data[HIS_MOSFET_MAXTEMP]=4000;
}

void BQ_Init(void)
{
	uchar bq_tmp[1];
//	int result;

    BQ76930Data.SysCtrl1.SysCtrl1Byte =  BQ76930_SYS_CTRL1_Default;
    BQ76930Data.SysCtrl2.SysCtrl2Byte = BQ76930_SYS_CTRL2_Default;
    BQ76930Data.Protect1.Protect1Bit.RSNS=1;
    BQ76930Data.Protect1.Protect1Bit.SCD_DELAY = SCD_DEALY_200us;
//    BQ76930Data.Protect1.Protect1Bit.SCD_THRESH = SCD_THRESH_200mV_100mV; 	//应客户要求修改 20190801 fenglong SCD_TRHESH_155mV_68mV;
    BQ76930Data.Protect1.Protect1Bit.SCD_THRESH = SCD_THRESH_111mV_56mV;   //44A
    BQ76930Data.Protect1.Protect1Bit.RSVD=0;
    BQ76930Data.Protect2.Protect2Bit.RSVD=0;
    BQ76930Data.Protect3.Protect3Bit.RSVD=0;
    BQ76930Data.Protect2.Protect2Bit.OCD_DELAY = OCD_DELAY_40ms;
//    BQ76930Data.Protect2.Protect2Bit.OCD_THRESH = OCD_THRESH_67mV_33mV;
    BQ76930Data.Protect2.Protect2Bit.OCD_THRESH = OCD_THRESH_67mV_33mV;			//27A
    BQ76930Data.Protect3.Protect3Bit.OV_DELAY = OV_DELAY_2s;     //OV_DELAY_2s;      fenglong 20190612
    BQ76930Data.Protect3.Protect3Bit.UV_DELAY =  UV_DELAY_1s;      //fenglong 20190705
    BQ76930Data.CCCfg = 0x19;

    I2C_Master_TransmitByte( BQ76930_SYS_CTRL1_Addr,    BQ76930Data.SysCtrl1.SysCtrl1Byte);
    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 1, BQ76930Data.SysCtrl2.SysCtrl2Byte);
    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 2, BQ76930Data.Protect1.Protect1Byte);
    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 3, BQ76930Data.Protect2.Protect2Byte);
    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 4, BQ76930Data.Protect3.Protect3Byte);

//    BQ76930Data.OVTrip = 0xB7;//4.25v		10 1011 0111 1000   *382/1000;
//    BQ76930Data.UVTrip = 0x9E;//2.53v		01 1001 1110 0000	*382/1000;
	BQ76930Data.OVTrip = 0xAF;//4.2v		10 1011 0111 1000   *382/1000;
	BQ76930Data.UVTrip = 0xCA;//2.8v		01 1001 1110 0000	*382/1000;

    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 5, BQ76930Data.OVTrip);
    I2C_Master_TransmitByte(BQ76930_SYS_CTRL1_Addr + 6, BQ76930Data.UVTrip);
    I2C_Master_TransmitByte( BQ76930_SYS_CTRL1_Addr + 7,BQ76930Data.CCCfg);

//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr, 1) ==0)		//C_CCFG
    {
        if(bq_tmp[0] != BQ76930Data.SysCtrl1.SysCtrl1Byte)
        {
            BQ76930Data.CCCfg =0;
        }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+1, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+1, 1) ==0)
    {
        if(bq_tmp[0] != BQ76930Data.SysCtrl2.SysCtrl2Byte)
        {
            BQ76930Data.CCCfg =0;
        }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+2, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+2, 1) ==0)
    {
        if(bq_tmp[0] != BQ76930Data.Protect1.Protect1Byte)
        {
            BQ76930Data.CCCfg =0;
        }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+3, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+3, 1) ==0)
    {
        if(bq_tmp[0] != BQ76930Data.Protect2.Protect2Byte)
        {
            BQ76930Data.CCCfg =0;
        }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+4, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+4, 1) ==0)
    {
       if(bq_tmp[0] != BQ76930Data.Protect3.Protect3Byte)
       {
           BQ76930Data.CCCfg =0;
       }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+5, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+5, 1) ==0)
    {
       if(bq_tmp[0] != BQ76930Data.OVTrip)
       {
           BQ76930Data.CCCfg =0;
       }
    }
//    result = I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+6, 1);//C_CCFG
    if(I2C_Load_BQ76930(&bq_tmp[0],BQ76930_SYS_CTRL1_Addr+6, 1) ==0)
    {
       if(bq_tmp[0] != BQ76930Data.UVTrip)
       {
           BQ76930Data.CCCfg =0;
       }
    }
//    Get_Gain();
//    GetADCGainOffset();
//    iGain = 365 + ((BQ76930Data.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((BQ76930Data.ADCGain2.ADCGain2Byte & 0xE0)>> 5);
}

void Get_Gain(void)
{
//	iGain=382;
	GetADCGainOffset();
	iGain = 365 + ((BQ76930Data.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((BQ76930Data.ADCGain2.ADCGain2Byte & 0xE0)>> 5);
}

void    SystemInitial (void)
{
    Calibration ();
    FCTL2 = FWKEY + FSSEL_1 + FN5;        //Set fFTG = MCLK / 32,

    GPIOInitial ();
    Timer_A_initial ();
    UART_Initial ();
    I2C_Initial ();
    ConfigWDT();
    SWInit();
    RAM_Init();
}









