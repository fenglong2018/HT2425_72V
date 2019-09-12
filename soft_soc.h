

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SOFT_SOC_H
#define __SOFT_SOC_H
#include "System.h"



#define CAP_CNT_VAL 		6985//14400/2   //1mah  7200  AID 20190605
#define CUR_DSG_005C   		48			// AD = I*200/844   200mA


//typedef union
//{
//    struct{
//        uint16_t    full_cap;   // ������    // ��ʱΪ�̶�ֵ
//        uint16_t    bat_cap;    // �������   // ��ʱδ��
//        int32_t     cap_val;    // ������       // ��ʹ����������ֵ����洢��FLASH���洢ʱתΪ��ֵ�洢
//        int32_t     cap_val2;   // ������ 2  ���γ��������ۼ�ֵ   // ��ʱδ��
//
//        uint16_t    cycle_cap_val;  //ѭ��������������������ѭ����������洢��FLASH
//        uint16_t    cycle_dch_cap_val;  //   // ��ʱδ��
//
//        uint8_t     show_cap_rate;  // ��ʾ����ֵ �����������չʾ���û���SOCֵ����������ʣ�������ٷֱ���һ���������̣�������Ч�仯����洢��FLASH
//        uint8_t     re_cap_rate;    // ʣ�������ٷֱ�   // SOCr ��(CAPa - CAPu - CAPf)/CAPa�������
//        uint8_t     re_cap_rate2;   // ���ǰ�ľ���soc   // ��ʱδ��
//        uint8_t     re_cap_rate3;   // ���ǰ�ľ���soc ���ҵ�ѹС��3500mv   // ��ʱδ��
//
//        int32_t     cap_cnt;
//        uint16_t    bat_cycle_cnt;  // ѭ����������洢��FLASH
//        uint16_t    dch_cycle_cnt;   // ��ʱδ��
//    }val;
//}__SYS_CAP;
typedef union
{
    struct{
        long    cap_cnt;

        u16     full_cap;               // ������    // ��ʱΪ�̶�ֵ
        u16     bat_cap;                // ���ʣ������
        u16     bat_cap_fcc;
        u16     bat_cycle_cnt;          // ѭ����������洢��FLASH
        u8      re_cap_rate;            // ʣ�������ٷֱ�   // SOCr ��(CAPa - CAPu - CAPf)/CAPa�������
        u8      re_cap_rate_fcc;
        u8      re_cap_rate_volt;
        u8      chg_cap_cnt;            //������
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
