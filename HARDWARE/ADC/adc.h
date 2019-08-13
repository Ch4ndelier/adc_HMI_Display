#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"


void MY_ADC_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Max(u8 ch,u16 times); 
 
#endif 
