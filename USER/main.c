#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 

#include "math.h"
#include "adc.h"

#include "hmi.h"


 int main(void)
 {	 
  u16 votage_1;
	u16 adcxv;
	float Gain,temp,votage_one,votage_two,votage_three,votage_three_high=0,votage_three_low=0; 
	u16 votage_2;
	u16 votage_3;
  int Resis_out=0,Resis_in=0;
	int Resis_out_lowest=1000000;
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200	 
	uart3_init(115200);
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();			 	
 	MY_ADC_Init();		  		//ADC��ʼ��
	 
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	//LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	//LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	//LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(60,110,200,16,16,"2015/1/14");	
	//��ʾ��ʾ��Ϣ
	//POINT_COLOR=BLUE;//��������Ϊ��ɫ
	//LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	//LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	       
	while(1)
	{
		votage_1=Get_Adc_Max(ADC_Channel_1,200);
		votage_2=Get_Adc_Max(ADC_Channel_2,200);
		votage_3=Get_Adc_Max(ADC_Channel_3,200);
		
		//analog
		votage_one=(float)votage_1*(3.3/4096);
		votage_two=(float)votage_2*(3.3/4096);
		votage_three=(float)votage_3*(3.3/4096);
		
		//in
		Resis_in=(int)((votage_two/(votage_one-votage_two))*11550*7);
		
		
		//out
		if(votage_three>0.05)votage_three_high=votage_three;
		if(votage_three>0.00001&&votage_three<0.05)votage_three_low=votage_three;
		if(votage_three_low!=0&&votage_three_high!=0)Resis_out=((votage_three_high-votage_three_low)/votage_three_low)*2000/360;
		if(Resis_out<900000&&Resis_out>15)Resis_out_lowest=Resis_out;
		
		//1Hz Gain
		Gain=votage_three_high/votage_two;
		
		//LCD_ShowxNum(156,130,votage_1,4,16,0);//��ʾADC��ֵ
		temp=(float)votage_1*(3.3/4096);
		adcxv=temp;
		LCD_ShowxNum(156,150,adcxv,1,16,0);//��ʾ��ѹֵ
		temp-=adcxv;
		temp*=1000;
		//LCD_ShowxNum(172,150,temp,3,16,0X80);
		printf("adcx:%d\r\n",votage_1);
	  //printf("adcxv:%d\r\n",adcxv);	
		//printf("temp:%f\r\n",temp);
		
	HMI_SendNum(USART3,"n0.val=",7,votage_2,4);
	HMI_SendNum(USART3,"n1.val=",7,(int)adcxv,1);
	HMI_SendNum(USART3,"n2.val=",7,((int)temp)/100,1);
	HMI_SendNum(USART3,"n3.val=",7,(((int)temp)/10)%10,1);
	HMI_SendNum(USART3,"n4.val=",7,((int)temp)%10,1);
	
	
  HMI_SendNum(USART3,"n5.val=",7,(int)Resis_in,5);
	
	//send Resis_out
	if(Resis_out_lowest!=1000000){HMI_SendNum(USART3,"n6.val=",7,(int)Resis_out_lowest,5);HMI_SendText(USART3,"t2.txt=",7," ",10);}
  if(Resis_out_lowest==1000000)HMI_SendText(USART3,"t2.txt=",7,"�����غ��Ե�",12);	
	
	//send Gain
	HMI_SendNum(USART3,"n7.val=",7,(log10(Gain))*20,5);
	
	
		
	
	LED0=!LED0;
	delay_ms(1000);	
	}
 }

