#ifndef __main_H
#define __main_H

#define RotEnGpio 			GPIOD, GPIO_PIN_7 // ��������� �� ��������
#define RotDirGpio			GPIOD, GPIO_PIN_5 // ����� ����������� ��������
// #define RotPulseGpio GPIOD, GPIO_PIN_3 // ������������ ��������
#define LiftEnGpio			GPIOD, GPIO_PIN_0 // ��������� �� �������
#define LiftDirGpio			GPIOC, GPIO_PIN_6 // ����� ����������� �������
#define LiftPulseGpio 	GPIOC, GPIO_PIN_4 // ������������ �������


#define RotLedGpio    	GPIOB, GPIO_PIN_4 // ��������� ��������
#define LiftLedGpio   	GPIOB, GPIO_PIN_0 // ��������� �������

#define RotStartGpio		GPIOB, GPIO_PIN_5	// ������ ������ ��������
#define RotStopGpio 		GPIOC, GPIO_PIN_2 // ������ ���� ��������
#define LiftStopGpio 		GPIOC, GPIO_PIN_1 // ������ ���� �������
#define LiftStartGpio		GPIOF, GPIO_PIN_4	// ������ ����� �������

#define EncoderGpio 		GPIOB,GPIO_PIN_3,GPIOB,GPIO_PIN_2 // �������
#define RotPauseGpio 		GPIOB, GPIO_PIN_1 // ������ ��������

#endif