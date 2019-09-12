//************************************************************
//  Data        :   2019/04/09
//  Programmer  :   Garfield.Lin
//  Function    :
//  MCU         :   MSP430G2553IPW28
//  Oscillator  :   Internal DOC 12MHZ
//  Version     :   V0.1
//
/*
 *
 *     V1.1  20190521    AID   �޸�;
 *
 *
 *     V1.4   20190530 fenglong
 *      1. �ͻ�Ҫ��ͨ���ܹػ�
 *      2. �ͻ�Ҫ�����SOC�� δ������֤
 *      3. ����б����������޸�Ϊ150mA
 *      4. �����˵������͸�����书�ܣ�
 *      5. �����˸�����Ź���
 *      6. PF_OCC����Ϊ12A
 *      7. ����ͨ�ſ���ָʾ�Ʋ�������������������
 *
 *
 *      V1.5    20190611 fenglong
 *
 *       1. ���ӷŵ���ʼ�¶ȱ������趨61��
 *       2. �޸��˸���MOSFET����BUG
 *       3. 2.8V�������ϵ缤���ֹ�����ֱ����絽3.4V
 *       4. �����˵�о���µ��±�����MOSFET���±�������
 *
 *       5. ���й��ܲ���OK, ׼�����
 *
 *
 *      V1.6    20190615  fenglong
 *
 *       1.���ӳ����⣬ ����lifetimes ,CELLS max/min voltage record
 *       2. ������������д���ȡ��
 *       3. ���������̶�ȡ��
 *       2.�޸ķ���ʵʱ�����ṹ�弰���ͷ�ʽ���Խ�ʡRAM
 *       3. ����100ms��ʱ�޸�λ20ms
 *       4. �ر�4.17V ��ѹ���湦�ܡ�29.
 *       5. �����˵�����ʾ���ܡ�
 *       6. �޸ĳ����ܲ�ѯ����BUG
 *       7. �޸Ĳ�ѯ�������ܺ���BUG
 *
 *      V1.7    20190701 fenglong
 *       1. ����SLEEP,ʵ�⹦��С��150uA
 *
 *      V1.8    20190702 fenglong
 *       1. ���뱨����Ч
 *       2. ���������ػ���������߰���ʧЧBUG
 *       3. �޸Ŀ�����Ч
 *       4.
 *
 *
 *���ģ�  1  �������
 *���ģ� 2. ��Ч����
 *���ģ� 3 .ͨ����PF
 *
 *      V1.81    20190708 fenglong
 *       1. ��������޷�����BUG
 *       2. ������䰴�ͻ�Ҫ���޸�Ϊ����ƾ���
 *       3. ����ͨ�����PF
 *       4. �޸���д����������ָ����
 *
 *      V1.82    20190708 fenglong
 *       1. �������һ����ѹ�޷���ʾ����BUG
 *		 2. ������������޷�����			20190709
 *
 *      V1.83    20190709 fenglong
 *       1. ȡ�����������������
 *		 2. ɾ������û���õĲ�ѯ��������
 *
 *
 *       V1.84    20190709 fenglong 20190710
 *       1. ���Ŀ�����ˮ��Ч
 *		 2. ���� д�����޷��� DD
 *
 *       V1.86    20190713 fenglong 20190713
 *       1. ����������ˮ��ЧBUG
 *		 2. ���� д�����޷��� DD
 *		 3. �ĵ�ѹ���,����10����ʱ���
 *
 *        V1.87    20190714 fenglong 20190714
 *       1. Ӧ�ͻ�Ҫ���ֳ����Ŀ�����ˮ��Ч *
 *		 2. ���� д�����޷��� DD
 *
 *         V1.88    20190714 fenglong 20190714
 *       1. Ӧ�ͻ�Ҫ���ֳ�����SOC���ʱ��
 *       2. �������һ����ѹ��SHIPPING
 *		 2. ���� д�����޷��� DD
 *
 *        V1.89 ܛ������,ܛ���޸�PTǷ�����o���ܻ֏�BUG
 *
 *
 *
 * 		  V2.00    20190801
 *		 1. Ӳ���Ե�LED3,LED4���ţ���Ӧ��������
 *		 2. ������ѭ����������Ϊ�ۼƳ�90%Ϊһ��ѭ��
 *		 3. ��PF������Ч��Ϊ������ȼ�
 *		 4. �޸��˷ŵ���±������������Ʋ���BUG
 *		 5. Ӧ�ͻ�Ҫ���޸Ķ�·��������Ϊ80A
 *		 6. Ӧ�ͻ�Ҫ���޸�һ������Ϊ25A
 *
 *
 *
 *
 *	20190828 fenglong
 *	�������������36.38ms
 *
*	˯��CPU����0.040mv/10o=0.004mA=4uA
 *    ���CPU����54.940mv/10o=5.5mA
 *  �ŵ�CPU����53.595mv/10o=5.3mA
 *	�ŵ�+˯��CPU+LED������17.080mv/10o=1.7mA
 *	�ŵ�+˯��CPU+LED��������26.080mv/10o=2.6mA
 *
 *
 *��ѹ�幦��25V/5.8915mA, bqδ�����(5.9035-5.8915)mA, ˯�߹���(6.2970-5.8915)mA, �ŵ繦��(8.218-5.8915)mA��
 *
 */
//****************************************************************


//#include <msp430.h>
#include "System.h"
//#include "source/System.h"
#include"RS_485.h"
#include"uart.h"
#include"soft_soc.h"
//#include"string.h"

//extern _BatteryData BatteryData;
//extern UINT8 T0_INT_count;
uchar BQ_GetData_Delay =0;
//uchar Powrer_On_Delay =0;
//extern UINT16 CellMiniVoltage;
//extern UINT16 CellMAXiVoltage;
//extern UINT8 SystemMode;


//extern union FW_PT FW_PT_Status;               //CPU           һ������

extern uchar T0_20ms_count;
extern uchar T0_200ms_count;
extern uchar T0_1s_count;
//extern uchar T0_1m_count;

extern uchar T0_20ms_old;
extern uchar T0_200ms_old;
extern uchar T0_1s_old;
//extern uchar T0_1m_old;


void main(void)
{
	uchar temp=1;

    SystemInitial();

//    Calibration ();

//	initClockTo12MHz();
    FCTL2 = FWKEY + FSSEL_1 + FN5;        //Set fFTG = MCLK / 32,


    BQ_Init(); //init BQ Data

    __bic_SR_register(  GIE);       // Enter LPM3 w/ interrupt
    BlackBox_Init();
	__bis_SR_register(  GIE);       // Enter LPM3 w/ interrupt

	while(temp)
	{
		if(T0_200ms_count >= 3)
		{
			temp=0;
			T0_200ms_count=0;
			T0_20ms_count=0;
			T0_1s_count=0;
		}
		Get_Gain();
		WDTCTL = WDT_ARST_1000;             //fenglong 20190608
	}
	Update_BQ_Data();
	His_DataRenew();

	while(1)
	{
//		LED_2_Low;		//1ms:17ms
	    WDTCTL = WDT_ARST_1000;             //fenglong 20190608
	    ReadKey();          //detect key
		read_bq();
		RS485_REC_Task();
//		LED_2_High;


		if(T0_20ms_count != T0_20ms_old)  //20MS
		{
//			LED_2_High;			//20ms����һ�Σ��36.38ms
//			LED_2_Low;
			key_count();
			ProtectDetect();
			MosStatusDetect();
			SystemPF_Detect();
			LEDForBootUpMode ();
			TA11_Breathing();
//			RS485_REC_Task();

			T0_20ms_old = T0_20ms_count;
//			LED_2_Low;
		}
//		LED_5_Low;
		if(T0_200ms_count != T0_200ms_old)  //20MS
		{
//			LED_2_Low;
//			LED_2_High;			//ms:ms

			DelayTime200ms_count();
			AlarmDetect();
			NormalCapacityProc();
			SystemStatusDetect();
			BatteryRealData_Refresh();
//			sys_count();
			SleepMode();
			ShutDownMode();

			T0_200ms_old = T0_200ms_count;
		}
//		LED_5_High;
		if(T0_1s_count != T0_1s_old)  //
		{
//			LED_2_Low;
//			LED_2_High;
			DelayTime1s_count();
			ShipMode();
			balance();
			LoadFlashData();
			SleepCheck();
			ImbalanceCheck();

			T0_1s_old = T0_1s_count;
		}

		sleep_lmp0();
	}
}




