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
 *     V1.1  20190521    AID   修改;
 *
 *
 *     V1.4   20190530 fenglong
 *      1. 客户要求通信能关机
 *      2. 客户要求加入SOC， 未经过验证
 *      3. 充电判饱截至电流修改为150mA
 *      4. 增加了低温起充和高温起充功能，
 *      5. 增加了高温起放功能
 *      6. PF_OCC调整为12A
 *      7. 增加通信开机指示灯不亮，按键开机再亮。
 *
 *
 *      V1.5    20190611 fenglong
 *
 *       1. 增加放电起始温度保护，设定61度
 *       2. 修改了高温MOSFET保护BUG
 *       3. 2.8V保护后，上电激活禁止输出，直到充电到3.4V
 *       4. 增加了电芯高温低温报警，MOSFET高温报警功能
 *
 *       5. 所有功能测试OK, 准备封存
 *
 *
 *      V1.6    20190615  fenglong
 *
 *       1.增加充电均衡， 增加lifetimes ,CELLS max/min voltage record
 *       2. 增加生产日期写入读取。
 *       3. 增加制造商读取。
 *       2.修改发送实时参数结构体及发送方式，以节省RAM
 *       3. 按键100ms延时修改位20ms
 *       4. 关闭4.17V 过压警告功能。29.
 *       5. 增加了电量显示功能。
 *       6. 修改充电后不能查询电量BUG
 *       7. 修改查询电量不能呼吸BUG
 *
 *      V1.7    20190701 fenglong
 *       1. 加入SLEEP,实测功耗小于150uA
 *
 *      V1.8    20190702 fenglong
 *       1. 加入报警灯效
 *       2. 修正按键关机后进入休眠按键失效BUG
 *       3. 修改开机等效
 *       4.
 *
 *
 *待改：  1  高温起充
 *待改： 2. 灯效更改
 *待改： 3 .通信清PF
 *
 *      V1.81    20190708 fenglong
 *       1. 修正充电无法激活BUG
 *       2. 高温起充按客户要求修改为不红灯警报
 *       3. 增加通信清除PF
 *       4. 修改了写入生产日期指令码
 *
 *      V1.82    20190708 fenglong
 *       1. 修正充电一级过压无法显示满电BUG
 *		 2. 修正过温起放无法报警			20190709
 *
 *      V1.83    20190709 fenglong
 *       1. 取消虚拟电量补偿功能
 *		 2. 删除部分没有用的查询电量代码
 *
 *
 *       V1.84    20190709 fenglong 20190710
 *       1. 更改开机流水灯效
 *		 2. 待改 写日期无法改 DD
 *
 *       V1.86    20190713 fenglong 20190713
 *       1. 修正开机流水灯效BUG
 *		 2. 待改 写日期无法改 DD
 *		 3. 改电压查表,增加10秒延时查表
 *
 *        V1.87    20190714 fenglong 20190714
 *       1. 应客户要求现场更改开机流水灯效 *
 *		 2. 待改 写日期无法改 DD
 *
 *         V1.88    20190714 fenglong 20190714
 *       1. 应客户要求现场更改SOC查表时间
 *       2. 加入软件一级低压进SHIPPING
 *		 2. 待改 写日期无法改 DD
 *
 *        V1.89 軟件瘦身,軟件修改PT欠壓保護不能恢復BUG
 *
 *
 *
 * 		  V2.00    20190801
 *		 1. 硬件对调LED3,LED4引脚，相应的软件变更
 *		 2. 加入充电循环次数，定为累计冲90%为一个循环
 *		 3. 将PF报警等效改为最高优先级
 *		 4. 修改了放电高温保护后立即充电灯不亮BUG
 *		 5. 应客户要求修改短路保护电流为80A
 *		 6. 应客户要求修改一级过流为25A
 *
 *
 *
 *
 *	20190828 fenglong
 *	整个程序运行最长36.38ms
 *
*	睡眠CPU功耗0.040mv/10o=0.004mA=4uA
 *    充电CPU功耗54.940mv/10o=5.5mA
 *  放电CPU功耗53.595mv/10o=5.3mA
 *	放电+睡眠CPU+LED闪功耗17.080mv/10o=1.7mA
 *	放电+睡眠CPU+LED常亮功耗26.080mv/10o=2.6mA
 *
 *
 *分压板功耗25V/5.8915mA, bq未激活功耗(5.9035-5.8915)mA, 睡眠功耗(6.2970-5.8915)mA, 放电功耗(8.218-5.8915)mA、
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


//extern union FW_PT FW_PT_Status;               //CPU           一级保护

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
//			LED_2_High;			//20ms进来一次，最长36.38ms
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




