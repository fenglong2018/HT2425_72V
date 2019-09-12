/*
 * File:   UART.c
 * Author: AID
 *
 *   SOC
 * 
 * Created on 2016年9月22日, 下午5:46
 */
/****************************************************************************
 include files
*****************************************************************************/
#include "soft_soc.h"
/****************************************************************************
Private variables 
*****************************************************************************/



//extern UINT16 max_temp;
//extern UINT16 min_temp;

extern union HW_PT HW_PT_Status;       //BQ76930       二级保护
//extern union FW_PT FW_PT_Status;       //CPU           一级保护
volatile SystemCap  sys_cap;
unsigned char first_soc_cnt=0;
unsigned char deta_cap_cnt=0;
UINT16 re_cap_tmp;
UINT16 bat_cycle_tmp = 3000;
unsigned char  power_first_flag =0;
//SINT16 ov_cap_val =0;                     //fenglong 20190615
//UINT16 fcc_delay_cnt=0;                   //fenglong 20190615
extern _BatteryData BatteryData;
extern UINT8 SystemMode;
//UINT32 fcc_tmp;                           //fenglong 20190615
extern UINT16 CellMiniVoltage;
extern UINT8 DC_IN_Count;
extern UINT16 his_data[FLASH_DATA_NUM];
extern UINT8 BatteryCapacityRefreshCount;

//const u16  Cell_volt[101]=
// {
//	 2750,2800,3000,3120,3200,3260,3280,3300,3320,3334,
//	 3346,3357,3368,3382,3393,3408,3421,3433,3445,3456,
//	 3467,3477,3484,3492,3500,3507,3514,3521,3527,3533,
//	 3539,3545,3550,3556,3560,3566,3571,3577,3582,3587,
//	 3593,3598,3604,3609,3615,3621,3628,3635,3641,3649,
//	 3656,3665,3673,3683,3692,3703,3714,3725,3737,3749,
//	 3760,3771,3781,3791,3801,3811,3820,3829,3837,3846,
//	 3854,3862,3870,3879,3887,3896,3905,3914,3923,3933,
//	 3943,3954,3965,3976,3987,3999,4010,4021,4031,4042,
//	 4051,4059,4067,4075,4083,4091,4101,4111,4124,4142,
//	 4160
// };//OLD  fenglong 20190713

const u16  Cell_volt[101]=
{
 	 2800,
     2837,2886,2966,3024,3079,3125,3172,3216,3255,3290,
     3321,3350,3383,3406,3419,3428,3436,3443,3452,3459,			//25以下没有跳
     3470,3488,3498,3510,3522,3531,3539,3550,3559,3564,			//30掉25
     3570,3576,3581,3586,3591,3597,3602,3607,3613,3618,
     3624,3630,3636,3642,3649,3656,3663,3671,3679,3687,     //59
     3696,3705,3715,3724,3734,3743,3753,3763,3773,3784,
     3794,3805,3816,3827,3838,3848,3857,3866,3875,3884,
     3892,3901,3911,3921,3932,3944,3955,3968,3980,3993,
     4005,4016,4027,4036,4045,4051,4056,4060,4064,4069,     //89
     4072,4079,4089,4103,4125,     4132,4139,4146,4153,4160        //后面5位为手动加入
};




 
/****************************************************************************
FUNCTION		: SOC
DESCRIPTION		: ????(???)
INPUT			: None
OUTPUT			: None
NOTICE			: TMIER????
DATE			: 2016/06/24
*****************************************************************************/
void Soc_AddSub(void)
{
//    static uchar cap_rate_tmp=0;
	//static s8 temp_cap_buf =0;
	int tmp_cap=0;
    u16 cur;
	//u16 tmp_16;

	if(BatteryData.CC.CCWord >0x8000)
	{
	    cur = 0-BatteryData.CC.CCWord;
		sys_cap.val.cap_cnt -= cur;
		/*
	    //20190606 AID 增加虚拟容量补偿，采用电流线性补偿
	    if((CellMiniVoltage <3300)&&(sys_cap.val.re_cap_rate >15))
	    {
	        if(cur>2000)
	        {
//                k_value = (unsigned long)cur*21;
//                k_value-= 22222;
//                k_value = k_value/10;
	            //k_value = cur;
                cap_rate_tmp = VbatToSoc((u16)CellMiniVoltage);
                if((cap_rate_tmp+4) <sys_cap.val.re_cap_rate)
                {
                    if(cur>18000)
                    {
                        sys_cap.val.cap_cnt -= 40000;
                    }
                    else if(cur>15000)
                    {
                       sys_cap.val.cap_cnt -= 30000;
                    }
                    else if(cur >10000)
                    {
                        sys_cap.val.cap_cnt -= 20000;
                    }
                    else
                    {
                        sys_cap.val.cap_cnt -= 10000;
                    }
                }
	        }
	        sys_cap.val.cap_cnt -= cur; //
	    }
	    else
	    {
	        sys_cap.val.cap_cnt-=cur;
	    }
	    //ADD END*/
	}
	else
	{
	    sys_cap.val.cap_cnt += BatteryData.CC.CCWord;  //fenglong 20190604

	}

	tmp_cap=(int)(sys_cap.val.cap_cnt/CAP_CNT_VAL);  
	sys_cap.val.cap_cnt=sys_cap.val.cap_cnt-(long)(tmp_cap)*CAP_CNT_VAL;
    
		
	if(tmp_cap>-50)
	{
				
		if(sys_cap.val.bat_cap >30)
		{
			sys_cap.val.bat_cap+=tmp_cap;
		}
		else
		{
			if(tmp_cap>0)
			{
				sys_cap.val.bat_cap+=tmp_cap;
			}
		}
	}
	//补偿介入
//	if(sys_uarts.flag.cap_update_sub_flag == 1)
//	{
//			if(sys_cap.val.bat_cap >50)
//			{
//				sys_cap.val.bat_cap-= 50;
//			}
//			sys_uarts.flag.cap_update_sub_flag =0;
//	}
//	if(sys_uarts.flag.cap_update_add_flag == 1)
//	{
//			sys_cap.val.bat_cap+=50;
//			sys_uarts.flag.cap_update_add_flag =0;
//	}
	
	//如果需要满电前SOC不能跳转100%,增加此代码
	//if((protects.protect.chg_ovp_flag == 0)&&(protects.protect.chg_full_flag ==0))
	//{
//			if(sys_cap.val.bat_cap >= (sys_cap.val.full_cap - 5))
//			{
//					sys_cap.val.bat_cap=sys_cap.val.full_cap - 6;
//			}
	//}
	
}


u8 VbatToSoc(u16 bat_val)
{
	u8 i;
	for(i=0;i<101;i++)
	{
			if(bat_val <Cell_volt[i])
			{
		
					return i;
			}
	}
	return 100;
}



/****************************************************************************
FUNCTION		: NormalCapacityProc
DESCRIPTION		: 容量更新以及电压补偿
INPUT			: None
OUTPUT			: None
NOTICE			:
DATE			: 2018/01/31
*****************************************************************************/
void NormalCapacityProc(void)
{

	    u32 capacity_volt;
	    

        if( power_first_flag==0 )
        {
        	if( BatteryCapacityRefreshCount> 2 )
        	{
                capacity_volt = CellMiniVoltage;
                //sys_cap.val.re_cap_rate = VbatToSoc((u16)capacity_volt);
                if(first_soc_cnt ==0)
                {
                        re_cap_tmp = VbatToSoc((u16)capacity_volt);
                }
                else
                {
                        re_cap_tmp += VbatToSoc((u16)capacity_volt);
                }
                first_soc_cnt++;
                if(first_soc_cnt >3)
                {
                    re_cap_tmp >>=2;
                    if((sys_cap.val.re_cap_rate > (re_cap_tmp+2))||(sys_cap.val.re_cap_rate <(re_cap_tmp-2)))
                    {
                        sys_cap.val.re_cap_rate = re_cap_tmp;
                        capacity_volt = (u32)sys_cap.val.re_cap_rate*sys_cap.val.full_cap;
                        capacity_volt = capacity_volt/100;
                        sys_cap.val.bat_cap = (u16)capacity_volt;
                    }
                    power_first_flag=1;
                    first_soc_cnt =0;
                    deta_cap_cnt = sys_cap.val.re_cap_rate;
//                    ov_cap_val =0;



                }
        	}
        }
		else
        {
            //AID  20180710  增加补偿
            //Cap_Update_Check();//检测是否进行容量补偿和更新

            //FullCap_Update();//检测是否进行满电容量更新

            if(sys_cap.val.bat_cap >sys_cap.val.full_cap)
            {
//                    ov_cap_val += sys_cap.val.bat_cap - sys_cap.val.full_cap;
                    sys_cap.val.bat_cap = sys_cap.val.full_cap;
            }
            if(SystemMode==SystemChargeMode)
            {
                if((BatteryData.VBat.VBatWord >=CHG_VOL_FULL) && (BatteryData.CC.CCWord < CHG_CC_FULL))
                {
                    sys_cap.val.bat_cap = sys_cap.val.full_cap;
                }
            }
            if(( HW_PT_Status.Bits.HW_OV == 1 ) || (HW_PT_Status.Bits.HW_OV == 1))

            {
                sys_cap.val.bat_cap = sys_cap.val.full_cap;
            }

            capacity_volt = (u32)sys_cap.val.bat_cap*100;
            sys_cap.val.re_cap_rate = (u8)(capacity_volt/ sys_cap.val.full_cap);

            BatCycleProc();

        }
}

/****************************************************************************
FUNCTION		: Cap_Update_Check
DESCRIPTION		: 容量补偿更新程序
INPUT			: None
OUTPUT			: None
NOTICE			: 利用静置超过10分钟,进行容量补偿
DATE			: 2017/06/24
*****************************************************************************/

//void Cap_Update_Check(void)
//{
//		u32 capacity_volt;
//		u8 deta_cap;
//		u16 cur_tmp =0;
//		static u16 update_delay =0;
//		if(BatteryData.CC.CCWord >0x8000)
//		{
//				cur_tmp = cc;
//				cur_tmp = 0-cur_tmp;
//		}
//
//		if((cur_tmp <CUR_DSG_005C)&&(sys_flags.flag.adapter_connect_flag ==0)&&(sys_flags.flag.sys_comm_open_dch_flag ==1))
//		{
//				update_delay++;
//				if(update_delay >6000) //测试降低延迟    200 = 10S  6000 = 5min
//				{
//					update_delay = 0;
//					capacity_volt = vbat;
//					capacity_volt = capacity_volt*383/2500;
//					sys_cap.val.re_cap_rate_volt = VbatToSoc((u16)capacity_volt);
//					if(sys_cap.val.re_cap_rate_volt > sys_cap.val.re_cap_rate)
//					{
//							sys_uarts.flag.cap_update_sub_flag =0;
//							deta_cap = sys_cap.val.re_cap_rate_volt - sys_cap.val.re_cap_rate;
//							if(deta_cap >3)
//							{
//									sys_uarts.flag.cap_update_add_flag =1;
//							}
//					}
//					else
//					{
//							sys_uarts.flag.cap_update_add_flag =0;
//							deta_cap = sys_cap.val.re_cap_rate - sys_cap.val.re_cap_rate_volt;
//							if(deta_cap >3)
//							{
//									sys_uarts.flag.cap_update_sub_flag =1;
//							}
//					}
//				}
//		}
//		else
//		{
//				update_delay =0;
//		}
//}

/****************************************************************************
FUNCTION		: FullCap_Update
DESCRIPTION		: 满电容量更新
INPUT			: None
OUTPUT			: None
NOTICE			: 每次电量小于30%的充电,一直充到满电更新一次满电容量
							FCC更新逻辑,当电池从充电前到充满以后,满电-chg_cap= deta_mah.
						100-cap_rate = deta_rate.                  fc = deta_mah*100/deta_rate  
						ex:fc = 6400*100/50 = 12800 
DATE			: 2018/08/24
*****************************************************************************/
//void FullCap_Update(void)
//{

//
//		if(sys_states.state.fcc_power_on_flag ==1)
//		{
//				if((power_first_flag ==1)&&(sys_cap.val.re_cap_rate <60))  //30
//				{
//						if((max_temp <5500)&&(min_temp >3500))// 10~35℃
//						{
//								if((sys_flags.flag.chg_en_flag ==0)&&(sys_flags.flag.dch_en_flag == 0))
//								{
//										if(sys_states.state.soc_fcc_save_flag ==0)
//										{
//												sys_states.state.soc_fcc_save_flag = 1;
//												sys_cap.val.bat_cap_fcc = sys_cap.val.bat_cap;
//												sys_cap.val.re_cap_rate_fcc = sys_cap.val.re_cap_rate;
//										}
//								}
//						}
//				}
//		}
//		if(sys_states.state.soc_fcc_save_flag ==1)//&&(sys_flags.flag.sys_close_flag ==1))
//		{
//				if((protects.protect.chg_full_flag == 1)||(protects.protect.chg_ovp_flag ==1))
//				{
//						fcc_delay_cnt++;
//						if(fcc_delay_cnt >200)  //10S
//						{
//								fcc_delay_cnt =0;
//								if((sys_cap.val.bat_cap - sys_cap.val.bat_cap_fcc) > 2000)  //5AH
//								{
//										sys_states.state.soc_fcc_save_flag =0;
//										fcc_tmp = sys_cap.val.full_cap - sys_cap.val.bat_cap_fcc +ov_cap_val;
//										fcc_tmp = fcc_tmp*100;
//										fcc_tmp = fcc_tmp/(100 - sys_cap.val.re_cap_rate_fcc);
//
//										if(fcc_tmp >  sys_cap.val.full_cap)
//										{
//												if((fcc_tmp - sys_cap.val.full_cap) < 2000)//1000mAH
//												{
//														AP_Flash_Write(fcc_tmp,(FLASH_CAP_ADDR+1 + sys_cap.val.fcc_index)); //sys_cap.val.fcc_index
//														sys_cap.val.fcc_index ++;
//														if(sys_cap.val.fcc_index >3)
//														{
//																sys_cap.val.fcc_index =0;
//														}
//														AP_Flash_Write(sys_cap.val.fcc_index,FLASH_CAP_ADDR);
//												}
//												else
//												{
//														protects2.protect.sys_fcc_err_flag =1;
//												}
//										}
//										else
//										{
//												if((sys_cap.val.full_cap - fcc_tmp) < 2000)//1000mAH
//												{
//														AP_Flash_Write(fcc_tmp,(FLASH_CAP_ADDR+1 + sys_cap.val.fcc_index));//sys_cap.val.fcc_index
//														sys_cap.val.fcc_index ++;
//														if(sys_cap.val.fcc_index >3)
//														{
//																sys_cap.val.fcc_index =0;
//														}
//														AP_Flash_Write(sys_cap.val.fcc_index,FLASH_CAP_ADDR);
//												}
//												else
//												{
//														protects2.protect.sys_fcc_err_flag =1;
//												}
//										}
//								}
//								else
//								{
//										sys_states.state.soc_fcc_save_flag =0;
//										protects2.protect.sys_fcc_err_flag =1;
//								}
//						}
//				}
//				else
//				{
//						fcc_delay_cnt =0;
//				}
//		}
//		else
//		{
//				fcc_delay_cnt =0;
//		}
//}

void BatCycleProc(void)
{
    UINT16 crc16;
    UINT8 i;
    if (DC_IN_Count >= TA0_3s)  //适配在位检测
    {
		if(sys_cap.val.re_cap_rate > deta_cap_cnt)
		{
			sys_cap.val.chg_cap_cnt += sys_cap.val.re_cap_rate - deta_cap_cnt;
			deta_cap_cnt = sys_cap.val.re_cap_rate;
			his_data[CHG_CAP_CNT] = sys_cap.val.chg_cap_cnt;

			if(sys_cap.val.chg_cap_cnt >=90)             //fenglong 20190615
			{
				//AP_Flash_Write(0,FLASH_CHGCAP_ADDR);
				sys_cap.val.chg_cap_cnt =0;
				sys_cap.val.bat_cycle_cnt++;
				his_data[FLASH_CYCLES_ADDR] = sys_cap.val.bat_cycle_cnt;  //写入到FLASH  fenglong 20190615  初始化需要读出来赋值
				his_data[CHG_CAP_CNT] = sys_cap.val.chg_cap_cnt;
				crc16 =0;
				for(i=0;i<FLASH_DATA_NUM-1;i++)
				{
					crc16 += his_data[i];
				}
				his_data[HIS_CRC16] = crc16;
				__bic_SR_register(  GIE);       // Enter LPM3 w/ interrupt
				Falsh_Write_Arr((unsigned int *)Segment_C,his_data,FLASH_DATA_NUM);
				Falsh_Write_Arr((unsigned int *)Segment_D,his_data,FLASH_DATA_NUM);
				__bis_SR_register(  GIE);       // Enter LPM3 w/ interrupt
			}
		}
    }
}



