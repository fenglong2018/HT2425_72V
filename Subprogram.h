#ifndef __SUBPROGRAM_H__
#define __SUBPROGRAM_H__



#if 1       //fenglong 20190617
unsigned int Flash_Read_int(unsigned int *start_addr,uint8_t index);   //read one byte
void Falsh_Write_Arr(unsigned int *Addr,unsigned int nValue[],unsigned char n);
void LoadFlashData(void);
#endif
void LED_ALARM(void);
void TA11_Breathing (void);
void LEDForBootUpMode (void);
void Update_BQ_Data(void);
void PT_ProtectRenew(void);
void BQ_Shutdown(void);
void ShipMode(void);
//void T0Counter (void);
void GetADCGainOffset();

void SystemStatusDetect (void);
void MosStatusDetect (void);
void SWInit(void);
void ReadKey(void);
void His_DataRenew(void);
void CLR_PF(void);
//void Test (void);
#endif
