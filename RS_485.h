/*
 * RS_485.h
 *
 *  Created on: 2019
 *      Author: 20190211012
 */


//#include "stdint.h"
//#include <stdio.h>
#include "system.h"


#ifndef RS_485_H_
#define RS_485_H_

#define CMD_ADD				0X03
#define CMD_PRO_LEN			0X0A

#define CRC_HEAD 			0X01
#define CRC_ADD1 			0X0B
#define CRC_ADD2 			0X0C

//#define RD_PRO_PARA				0X3031
#define RD_REAL_PARA			0X3032
//#define WR_FET_PARA				0X3036
//#define RD_VER_PARA				0X3039
//#define RD_CAP_PARA				0X3130
//#define RD_SN_PARA				0X3132
//#define RD_SOC_PARA				0X3133
#define WR_SHIP_PARA    		0x3134
//#define RD_MANUFACTURER         0x3135          //fenglong 20190617
//#define RD_PRODUCTION_DATA      0x3136          //fenglong 20190617
//
//#define WR_PRODUCTION_DATA      0x3237          //fenglong 20190708
#define CLR_PF_DATA				0X3238			//fenglong 20190706


//#define RD_PRO_NUM      0X00EA
//#define RD_REAL_NUM     0X0084
//#define WR_FET_NUM      0X0010
//#define RD_VER_NUM      0X0011
//#define RD_CAP_NUM      0X001E
//#define RD_SN_NUM       0X0016
//#define RD_SOC_NUM      0X0010

#define CELL_NUM    0X07
#define TEMP_NUM    0X02

#define BMS_VER     0X52

//#define BMS_SN      0XDH48V2

//#define VER14




typedef struct _BatteryRealData_
{
//    uchar   Addr   ;              //fenglong 20190617
//    uchar   cmd    ;
//    uchar   ver    ;
//
//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }Len;
//
//
////
////    uint_union    Len    ;
//    struct
//    {
//        uchar BMS_year;
//        uchar BMS_month;
//        uchar BMS_data;
//        uchar BMS_week;
//        uchar BMS_hour;
//        uchar BMS_minute;
//        uchar BMS_second;
//    }Time_t;


    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }BMS_Vbat;

//    uint    BMS_Vbat;
//    uchar   Vcell_num;            //fenglong 20190617


    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }Vcell[CELL_NUM];

//    uint    Vcell[CELL_NUM];

    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    } Curr[2];

//    uchar   TempNum;

    uchar   Temp[TEMP_NUM];


    union
    {
            struct _VSTATE_
            {
                uint    VOV:    1;
                uint    VUV:    1;
                uint    BVOV:   1;
                uint    BVUV:   1;
                uint    wVOV:   1;
                uint    wVUV:   1;
                uint    wBVOV:  1;
                uint    wBVUV:  1;

                uint    VDIFF:  1;
                uint    VBREAK: 1;
                uint    CSGDIS: 1;
            }bit;
        uint   val;
    } _VSTATE;


    union
    {
            struct _CSTATE_
            {
                        uint    CING:    1;
                        uint    DING:    1;
                        uint    OCCSG:   1;
                        uint    SHORT:   1;
                        uint    OCDSG1:  1;
                        uint    OCDSG2:  1;
                        uint    wOCCSG:  1;
                        uint    wOCDSG:  1;
            }bit;
        uint   val;
//        UINT8 val;
    } _CSTATE;



    union
    {
        struct _TSTATE_
        {
            uint    TCELL_CSGH:     1;
            uint    TCELL_CSGL:     1;
            uint    TCELL_DSGH:     1;
            uint    TCELL_DSGL:     1;
            uint    TENV_H:         1;
            uint    TENV_L:         1;
            uint    TFET_H:         1;
            uint    TFET_L:         1;

            uint    wTCELL_H:       1;
            uint    wTCELL_L:       1;
            uint    wTENV_H:        1;
            uint    wTENV_L:        1;
            uint    wTFET_H:        1;
            uint    wTFET_L:        1;

        }bit;
        uint   val;
    } _TSTATE;


    union
    {
        struct _ALARM_
        {
            uint    bit0:               1;
            uint    bit1:               1;
            uint    bit2:               1;
            uint    SPI_ERR:            1;
            uint    E2PROM_ERR:         1;
            uint    bit5:               1;
            uint    FCC_UPDATING:       1;
            uint    FCC_DSGLEARN:       1;
        }bit;
        uint   val;
    } _ALARM;


    union
    {
        struct _FETSTATE_
        {
            uint    DFET:               1;
            uint    CFET:               1;
            uint    SDFET:              1;
            uint    SCFET:              1;
            uint    DFET_DAMAGE:        1;
            uint    CFET_DAMAGE:        1;
            uint    CCFET:              1;
        }bit;
        uchar   val;
    } _FETSTATE;



//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }NUM_VOV;
    uchar NUM_VOV;

//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }NUM_VUV;
    uchar NUM_VUV;

//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }NUM_WARN_VHIGH;
    uchar NUM_WARN_VHIGH;


//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }NUM_WARN_VLOW;
    uchar NUM_WARN_VLOW;

//    union
//    {
//        struct
//        {
//            uchar high;
//            uchar low;
//        }len_byte;
//        uint   val;
//    }BlanceState;
    uchar BlanceState;

    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }DchgNum;

    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }ChgNum;

    uchar   SOC;

    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }CapNow;

    union
    {
        struct
        {
            uchar high;
            uchar low;
        }len_byte;
        uint   val;
    }CapFull;

    uchar CRC;

}_BatteryRealData;

extern  uchar SFET;



//extern uint8_t usartRxbuffer[RX_BUFFER_SIZE];

void BatteryRealData_Refresh(void);
void Dis_SOC(void);
uint8_t crc8_cal(uint8_t *prt, uint8_t len);

void RS485_REC_Task(void);
void pro_para_tx(uint16_t *p, uint8_t num);
void RS485_Char_TX(int8_t data);
uint U16_SWAP(uint data);
uchar ASC2HEX(uchar data1, uchar data2);

#endif /* RS_485_H_ */
