 #include "adc.h"
 #include "delay.h"
 #include "usart.h"	

#define M 128
#define N 8
uint16_t   value[N][M];
u32 ADC_ConvertedValue;  


//???ADC1
void Adc1_Multi_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //??ADC1????
 
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //??ADC????6 72M/6=12,ADC????????14M
 
	//PA1 ??????????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
 
	ADC_DeInit(ADC1);  //??ADC1,??? ADC1 ????????????
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC????:ADC1???????
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;     //????????????
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;    //?????????????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//??????????????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	  //ADC?????
	ADC_InitStructure.ADC_NbrOfChannel = 1;	   //?????????ADC?????
	ADC_Init(ADC1, &ADC_InitStructure);	  //??ADC_InitStruct???????????ADCx????   
 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
 
	 // ??ADC?DMA??
	ADC_DMACmd(ADC1, ENABLE);  //??ADC?DMA?
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //??ADC1

	/* Enable ADC1 reset calibaration register  ??ADC1??????? */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register   ADC1???????????*/
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration    ??ADC1?? */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration  ADC1?????? */
	while(ADC_GetCalibrationStatus(ADC1));
		
}	


//???ADC2
void Adc2_Multi_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC2	, ENABLE );	  //??ADC2????
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //??ADC????6 72M/6=12,ADC????????14M
 
	//PB0,1 ??????????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//??????
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
 
	ADC_DeInit(ADC2);  //??ADC2,??? ADC2 ????????????
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC????:ADC1???????
	ADC_InitStructure.ADC_ScanConvMode =DISABLE; //????????????
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //?????????????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //??????????????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC?????
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//?????????ADC?????
	ADC_Init(ADC2, &ADC_InitStructure);	//??ADC_InitStruct???????????ADCx????   
 
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );

	ADC_ExternalTrigConvCmd(ADC2, ENABLE);   //??ADC2??????? 
 
    /* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE); //??ADC2

	/* Enable ADC1 reset calibaration register  ??ADC2??????? */   
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC1 reset calibration register   ADC2???????????*/
	while(ADC_GetResetCalibrationStatus(ADC2));

	/* Start ADC1 calibaration    ??ADC2?? */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC1 calibration  ADC2?????? */
	while(ADC_GetCalibrationStatus(ADC2));
	 
}


/*???ADC */
void MY_ADC_Init(void)
{
	Adc1_Multi_Init();
	Adc2_Multi_Init();
	
}


void task_adc(void)
{    
	int i;
     printf("\r\n ????\r\n");
	 for(i=0;i<M;i++)
		{
			/* Start ADC1 Software Conversion  ??ADC1???? */ 		
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //????
			
			ADC_ConvertedValue=ADC1->DR;
			
			value[0][i] = (ADC_ConvertedValue&0xffff); //??ADC??
   		    value[1][i] = ((ADC_ConvertedValue>> 16)&0xffff);  //??ADC??
			
			printf("\r\n ??%d  \t ??%d  \t AD?: %d\r\n", 0,i,  value[0][i]);
			printf("\r\n ??%d  \t ??%d  \t AD?: %d\r\n", 1,i,  value[1][i]); 
					
		}
		 printf("\r\n ????\r\n");
}


u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Max(u8 ch,u16 times)
{
	u32 temp_max=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_max+=Get_Adc(ch);
		delay_ms(2);
	}
	return temp_max/times;
} 	 



























