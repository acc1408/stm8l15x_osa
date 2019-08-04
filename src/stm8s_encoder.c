#ifndef __STM8S_ENCODER_C
#define __STM8S_ENCODER_C
#include "inc/stm8s_encoder.h"

void EncoderInit(encoder_t* encod, 
										GPIO_TypeDef* p1_Gpio, GPIO_Pin_TypeDef p1_Pin,
										GPIO_TypeDef* p2_Gpio, GPIO_Pin_TypeDef p2_Pin,
										int32_t cnt,int32_t boost_max,uint16_t period_max )
{
	encod->p1_gpio=p1_Gpio;
	encod->p1_pin=p1_Pin;
	encod->p2_gpio=p2_Gpio;
	encod->p2_pin=p2_Pin;
	encod->cnt=cnt;
	encod->boost_max=boost_max;
	encod->period=1;
	if (period_max>0)
	{
		encod->period_max=period_max;
		encod->period=period_max;
	}
	else
	{
		encod->period_max=1;
	}
	GPIO_Init(encod->p1_gpio, encod->p1_pin, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(encod->p2_gpio, encod->p2_pin, GPIO_MODE_IN_FL_NO_IT);
  encod->p1_last=GPIO_ReadInputPin(encod->p1_gpio, encod->p1_pin);
	encod->p2_last=GPIO_ReadInputPin(encod->p2_gpio, encod->p2_pin);
}


int32_t EncoderRead(encoder_t* encod)
{
	int8_t dir; // ����������� ���������
	int32_t dc; //��������� ��������
	if (encod->period<encod->period_max) 
	{
		encod->period++;
	}
	// ��������� �������� � �����
	encod->p1_now=GPIO_ReadInputPin(encod->p1_gpio, encod->p1_pin);
	encod->p2_now=GPIO_ReadInputPin(encod->p2_gpio, encod->p2_pin);
	
	// ��������� ����� ��� 1 ������
	if (encod->p1_last!=encod->p1_now) 
		{
			encod->p1_last=encod->p1_now;
			encod->p1_front=1;
			dir=1;
		}
		
	// ��������� ����� ��� 1 ������
	if (encod->p2_last!=encod->p2_now) 
		{
			encod->p2_last=encod->p2_now;
			encod->p2_front=1;
			dir=-1;
		}
		// ����������� ������ �������� 
	if ((encod->p1_now==encod->p2_now)&&(encod->p1_front)&&(encod->p2_front) )
		{
			
			encod->p1_front=0;
			encod->p2_front=0;
			dc=dir*encod->boost_max/encod->period;
			encod->period=0;
			if ((dc>0) && (encod->cnt>0))
			{
				encod->cnt+=dc;
				if (encod->cnt<0) { encod->cnt=S32_MAX; }
				return encod->cnt;
			}
			
			if ((dc<0) && (encod->cnt<0))
			{
				encod->cnt+=dc;
				if (encod->cnt>0) { encod->cnt=S32_MIN; }
				return encod->cnt;
			}
		}
		else 
		{
		dc=0;
		}
	encod->cnt+=dc;	
	return encod->cnt;
}

#endif