

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SOFT_SOC_H
#define __SOFT_SOC_H
#include "System.h"



#define CAP_CNT_VAL 		6985//14400/2   //1mah  7200  AID 20190605
#define CUR_DSG_005C   		48			// AD = I*200/844   200mA


//typedef union
//{
//    struct{
//        uint16_t    full_cap;   // 满容量    // 暂时为固定值
//        uint16_t    bat_cap;    // 电池容量   // 暂时未用
//        int32_t     cap_val;    // 容量池       // 已使用容量，负值，需存储到FLASH，存储时转为正值存储
//        int32_t     cap_val2;   // 容量池 2  单次充电的容量累计值   // 暂时未用
//
//        uint16_t    cycle_cap_val;  //循环次数容量，用来计算循环次数，需存储到FLASH
//        uint16_t    cycle_dch_cap_val;  //   // 暂时未用
//
//        uint8_t     show_cap_rate;  // 显示容量值 用来存放最终展示给用户的SOC值，与真正的剩余容量百分比有一个趋近过程，并不等效变化。需存储到FLASH
//        uint8_t     re_cap_rate;    // 剩余容量百分比   // SOCr 由(CAPa - CAPu - CAPf)/CAPa计算而来
//        uint8_t     re_cap_rate2;   // 充电前的静置soc   // 暂时未用
//        uint8_t     re_cap_rate3;   // 充电前的静置soc 并且电压小于3500mv   // 暂时未用
//
//        int32_t     cap_cnt;
//        uint16_t    bat_cycle_cnt;  // 循环次数，需存储到FLASH
//        uint16_t    dch_cycle_cnt;   // 暂时未用
//    }val;
//}__SYS_CAP;
typedef union
{
    struct{
        long    cap_cnt;

        u16     full_cap;               // 满容量    // 暂时为固定值
        u16     bat_cap;                // 电池剩余容量
        u16     bat_cap_fcc;
        u16     bat_cycle_cnt;          // 循环次数，需存储到FLASH
        u8      re_cap_rate;            // 剩余容量百分比   // SOCr 由(CAPa - CAPu - CAPf)/CAPa计算而来
        u8      re_cap_rate_fcc;
        u8      re_cap_rate_volt;
        u8      chg_cap_cnt;            //充电次数
        u8      fcc_index;
    }val;
}SystemCap;

extern volatile SystemCap  sys_cap;

void Soc_AddSub(void);
void NormalCapacityProc(void);
u8 VbatToSoc(u16 bat_val);
void Cap_Update_Check(void);
void FullCap_Update(void);
void BatCycleProc(void);

#endif /* __SPI_FLASH_AUTO_H -------------------------------------------------------------------------------*/
