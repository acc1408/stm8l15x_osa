/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
#include  <stdio.h>
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
//void cmdreset(void);
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
/*
SetLCD_t lcd1;

 char st[40]="Test";

bme280_t bm;
uint32_t pres;
uint32_t hum;
int32_t t;
uint8_t rez;
*/
/*
char st[50];
char st_tx[100]; //  ����� ��� ��������
static uint8_t num_st=0;
unsigned short kol_tx=0; // ������� ������ ��������
unsigned char tx_num=0; // ����� ���� ��� ��������
float tx_float=0; // ������ �������� ���������� �����
bool tx_start=0; // � ��������� ������ �������� ������ ���
char st_tx2[10];

void putchar_st(char c)
{
	st[num_st] = c;
	num_st = (num_st+1) % 50;
}
*/
// ���������� ���������� �������� ������
// ������ ������� ���������� � stm8s_it.c
/*
void handler_tx(void)
{
	UART2_SendData8(st_tx[tx_num]); // �������� ������ � ��������� �������
	tx_num++; // ����������� �������
	if (st_tx[tx_num]=='\0') { 
														UART2_ITConfig(UART2_IT_TXE, DISABLE); // ���������  ���������� ���� ��������� ���� ����� ������
														tx_num=0; // �������� ������� ��� ����������� ������
														tx_start=0; // ���������� ���� ��������,
														// ������ ������ � ������ �� �����
														}
}
*/

//uint8_t mass[400];
// ��������� ������� �����������
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// ��������� ������� ���������� �������� - ������� 
typedef struct
{
	char name[20];	// �������� �������
	func_t func;		// ��������� �� �������
} tablefunc_t;

// ������ ��������� ������
#define cmdrxnum_max 40
// ������������ ���-�� ����������
#define cmdargc_max 3
typedef enum
{
	receive=0, 	// ����� ������
	exec=0b01,			// ������ ����-����
	error_ovf=0b10, // ������������ ��������� ������
	exec_error_ovf=0b11 // ������� ����-��� � ���� ������������ ������
}cmdstate_t;

// ��� ��������� ��������� ������
typedef struct
{
	char rx[cmdrxnum_max]; // �����
	uint8_t rxnum; // ����� �������� �����
	char stopbit; // ����-���
	tablefunc_t *tabfunc; // ��������� �� ������ �������
	uint8_t numfunc;			// ���-�� �������
	cmdstate_t state; // ��������� ��������� ������
	uint8_t argc; // ���-�� ����������
	char *argv[cmdargc_max]; // ������ ���������� �� ��������� 
	//char stpstr[2];
}cmdline_t;


cmdline_t cmd; // ��������� ��������� ������
// ������������ ��������� ������
void cmdinit(	char stopbit, // ������ ������ ����-���
							tablefunc_t tabfunc[], //��������� �� ������ �������
							uint8_t numfunc) // ���-�� �������
{
	//uint8_t i=0;
	cmd.stopbit=stopbit; // ����-���
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
	//while(*st && i<2)
	//cmd.stpstr[i++]=*st++;
}

void cmdinputchar(char c)
{
	switch(cmd.state)
	{
		// ����� ������
		case receive:
				
				// ��������� ����� � ������
				if (cmd.rxnum<cmdrxnum_max) 
					cmd.rx[cmd.rxnum++]=c; // ��������� ����� 
				else 	
					{
					cmd.state=error_ovf; // ��������� ������ ������������ ������
					}
				//break;
		// ���� ��������� ������������ ������� ����-����
		case error_ovf:
				if (c==cmd.stopbit) // ���� ������� ����-���, 
						cmd.state|=exec; // ������ ��� �� ���������
				break;
		default:
					nop();
				break;
	}
}
// ������ �� �����, ������ �������� ��������� ����� ������
// � ������ ������� ��������� �� ������ ����
void cmdparcer(void)
{
	// �������� ��� ����� �������� �� ����� ������
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // ���� ������ ������
			cmd.rx[i]='\0'; // �� �������� ��� �� ����� ������
	cmd.rx[cmd.rxnum-1]='\0';
	// ���������� ������ �����
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx;
		}
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0')&&(cmd.argc<cmdargc_max))
			cmd.argv[cmd.argc++]=cmd.rx+i;
	nop();
}

// ����� ��������� �������� ��������� ������
void cmdreset(void)
{
	cmd.rxnum=0;
	cmd.state=receive;
	cmd.argc=0;
}

// ����� ������� ��� ����������
void cmdexec(void)
{
	uint8_t i;
	char *funcfind; // ��������� �� ������
	func_t funcexec;// ��������� �� �������
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // ��������� �� ������ ������
		funcexec=cmd.tabfunc[i].func; // ��������� �� ��������������� ������
		if (strcmp(cmd.argv[0],funcfind)==0) 
		{
			nop();
			funcexec(cmd.argc,cmd.argv);
			break; // ���� ������� �������, �� ������� �� ����� ������ 
		}
	}	
		cmdreset(); // ����� �������� ��� ����� ��������
}



// ������� ���� ��� ��������� ������� ������
void cmdmainloop(void)
{
	switch(cmd.state) // ��������� ��������� ������
		{
			case exec: // 
					cmdparcer(); 
					cmdexec();
				break;
			case 	exec_error_ovf:
					cmdreset();
				break;
		}
}

char st2[40]="test string"; // ������ ��� �������� ������

// ���������� � ��������� ����������
int16_t led(uint8_t argc,char *argv[])
{
	if (argc=2)  
	{
		if (strcmp(argv[1],"on")==0) // �������� ���������
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
		else 
			if (strcmp(argv[1],"off")==0)  // ��������� ���������
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
		return 0;
	}
	else
		return 1;
}


uint8_t bl=1; // ���������� ��� ��������� � ���������� �������
// ������� ��� ��������� � ���������� �������
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc=2) 
	{
		if (strcmp(argv[1],"on")==0) // ��������� ���������
			bl=1;
		else 
			if (strcmp(argv[1],"off")==0) // �������� ���������
				bl=0;
		return 0;
	}
	return 1;
}

// ������� ������������ ��������-������� ��������� �� � ���,
// � �� FLESH, �.�. ����� �� ������������� �� ���-�� �������
// � ����� �����
const tablefunc_t testf[2]={"led", led,
														"blink", blink	
														};
char simvol;														
char putchar(char c)
{
	nop();
	simvol=c;
	nop();
	return 0;
}

uint8_t CondFlag; // ���������� ��� �������� �������� ���������
void asm_insert(void)
{
	enableInterrupts(); // ��������� ���������� ����������
	// ������ ����������� ������
	#pragma asm			// ������ ������������ �������
		push CC 			// �������� ������� ��������� � ����
		pop _CondFlag // ��������� ������� ��������� � ��������� ����������
	#pragma endasm	// ����� ����������� �������
	disableInterrupts(); // ������������� ��������� ���������� ����������
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST); // �������� ���
	// ��� �����-�� ������� ������ �������������� ����������� ��� ����������
	#pragma asm			// ������ ������������ �������
		push _CondFlag	// �������� �������� ��������� � ����
		pop CC				// ��������� �������� ��������� �� ����� � ������� ���������
	#pragma endasm	// ����� ������������ �������
	// ����� ����������� ������
}

//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	I2C_Init_7bit(100000);
	//Lcdi2cInit(&lcd1, 0b0111111);
	Lcdi2cInit(&lcd1, 0b0111111, 
								ENABLE,
								DISABLE,
								ENABLE);
	LcdCursorSet(&lcd1, 0);
	Lcdi2cPrint(&lcd1, st);
	//LcdCursorRight(&lcd1);
	//LcdDisplayLeft(&lcd1);
	//I2C_MasterSendSend(0b0111111, a, 1, a+4, 3);
	//I2C_MasterSendReceive(0b0111111, a, 0, a+1, 4);
	
rez=BME280_Init(&bm, 0b1110110,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_FILTER_COEFF_8,
						BME280_STANDBY_TIME_250_MS,
						BME280_NORMAL_MODE
						);
	//BME280_Reset(&bm);
	//BME280_StartStop(&bm, BME280_NORMAL_MODE);
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
		rez=BME280_GetMeasurement(&bm);
		pres=BME280_compensate_P_int32(&bm);
		t=BME280_compensate_T_int32(&bm);
		hum=BME280_compensate_H_int32(&bm);
		a=t/100;
		b=t%100;
		CursorGoTo(&lcd1, 1, 0);
		sprintf(st,"Temp=%d.%02d C   ", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=pres/1000;
		b=pres%1000;
		LcdCursorSet(&lcd1, 20);
		sprintf(st,"Press=%d%03d Pa", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=hum/1024;
		b=hum%1024;
		if (b>999) b=999;
		LcdCursorSet(&lcd1, 60);
		sprintf(st,"Hum=%d.%03d%%   ", a,b);
		Lcdi2cPrint(&lcd1, st);
	}
}
#endif

#define cond_flag (((uint8_t *) 0x7f0a))


uint8_t *cf_u;
int16_t sdvig=0xa000;
uint16_t sdv2=0xF000;

void rotate_left (void)
{
uint8_t i;
for(i=0;i<4;i++) // ��������� �������� ����� ����� 4 ����
		{
			#pragma asm			// ������ ������������ �������
				LDW X,_sdvig 	// ��������� � ������� � �������� 16-������ ���������� �� ������ sdvig
				LDW Y,#$7FFF 	// ��������� ����� ��� �������� ������� ���� �� 1 ��� 0
				CPW Y,_sdvig 	// ���� ����� ������, ��� $7FFF, �� ��� �������� ����������� � ��� � �������� �� 
				RLCW X				// ���������� �������� ����� � ���� ���� �.
				LDW _sdvig,X	// ��������� �� ������ _sdvig �������� �� �������� �
			#pragma endasm	// ����� ������������ �������
		}
}

uint8_t ua=0xCD; // ����������� ���������� 	��������			205 ��� 0b11001101
int8_t  sa=0xCD; // �������� ����������		 	��������			-51 ��� 0b11001101
uint8_t ua_right; // ����������� ���������� ����� ������	 51 ��� 0b00110011
int8_t  sa_right; // �������� ����������		����� ������	-13 ��� 0b11110011
uint16_t ua_left; // ����������� ����������  ����� �����		 52 ��� 0b00110100
int16_t  sa_left; // �������� ����������		  ����� �����		 52 ��� 0b00110100
void main(void)
{
 #ifdef  __OSA__
 // ����������� ������� ������������ �� 16���
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // ������������� RTOS OSA
	TIM4_TimerOSA(1000); //����������� ���������� 500���
	OS_Task_Create(7, Task); // ������� ������
	OS_EI();   // ��������� ��� ����������
	OS_Run(); // ������ ���� RTOS OSA
#else
	/* Infinite loop */
//	cf_u=&cf;
	uint8_t i;
	Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  //testf={.name="nvvjhggvn",.func=yui};
	cmdinit('\r', // ����-������
					testf, // ��������� �� ������� ��������-�������
					2 ); 	// ���-�� ������� � �������
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
	
	
	ua_right=ua>>2; // ��������� ����� ������  51 ��� 0b00110011
	sa_right=sa>>2; // ��������� ����� ������ -13 ��� 0b11110011
	ua_left =ua<<2; // ��������� ����� �����   52 ��� 0b00110100
	sa_left =sa<<2; // ��������� ����� �����   52 ��� 0b00110100

	//cc=	cond_flag;
	{
		//_asm(" \n \n");
		//__cf=_asm("");
		/*
		#pragma asm
			push CC
			pop _cf
		#pragma endasm
		*/
		for(i=0;i<4;i++) // ��������� �������� ����� ����� 4 ����
		{
			#pragma asm			// ������ ������������ �������
				LDW X,_sdvig 	// ��������� � ������� � �������� 16-������ ���������� �� ������ sdvig
				LDW Y,#$7FFF 	// ��������� ����� ��� �������� ������� ���� �� 1 ��� 0
				CPW Y,_sdvig 	// ���� ����� ������, ��� $7FFF, �� ��� �������� ����������� � ��� � �������� �� 
				RLCW X				// ���������� �������� ����� � ���� ���� �.
				LDW _sdvig,X	// ��������� �� ������ _sdvig �������� �� �������� �
			#pragma endasm	// ����� ������������ �������
		}
		
		sdvig=sdvig>>4;
		sdv2=sdv2>>4;
		//cf=_asm("ld cf,a ");
		//_asm("ld a ");
	
	}
	enableInterrupts();
	//asm_insert();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	/*
	#pragma asm
		push _cf
		pop CC
	#pragma endasm
	//cc=	cond_flag;
		*/
	while (1)
  {
		cmdmainloop(); // ��������� ��������� ������
		printf("qwetr %d",0x54);
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(1000);
	}
#endif
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
