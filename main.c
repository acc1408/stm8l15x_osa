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

//
/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************


uint16_t cnt_on2=0;
//extern  i2cTask_t i2c_Task;
#define i2cBufferSize 5
uint8_t a[2]={0,2};
uint8_t b[4]={3,4,5,6};
I2C_TypeDef* i2c_reg=I2C;
//uint8_t busError=0;

typedef enum
{
	i2cTxTx=0x00,
	i2cTxRx=0x01
}i2cModeTxRx_t;
/*
typedef enum
{
	i2cIdle=0x00,
	i2cRun=0x01
	//i2cRunMaster=0x01,
}i2cStateTxRx_t;
*/
typedef enum
{
	i2cSuccess=0x00,
	i2cFault=0x01
}i2cResult_t;



typedef struct
{
  union
	{
		uint8_t SR1;       /*!< I2C status register 1 */
		struct
		{
			uint8_t sb:1; // start bit generation=1
			uint8_t addr:1; // addess sent=1 (master)/match=1 (slave)
			uint8_t btf:1;	// byte transfer=1
			uint8_t add10:1; // master has sent header 10-bit=1 address
			uint8_t stopf:1; //stop detection=1 (slave)
			uint8_t reserv_sr1:1; // reserved
			uint8_t rxne:1; //data register not empty=1 (receiver)
			uint8_t txe:1; // data register empty=1 (transmitters)
		};
	};
	union
	{
		uint8_t SR2;       /*!< I2C status register 2 */
		struct
		{
			uint8_t berr:1; // buss error=1
			uint8_t arlo:1; // arbitration lost=1
			uint8_t af:1; // acknowledge failure=1
			uint8_t ovr:1; // overrun or underrun=1
			uint8_t reserv_sr2:1; // reserved
			uint8_t wufh:1;	// wakeup from Halt=1
			uint8_t reserv2_sr2:2;// reserved
		};
	};
	union
	{
	uint8_t SR3;       /*!< I2C status register 3 */
	struct
		{
			uint8_t msl:1; // master=1 / slave=0
			uint8_t busy:1; //bus busy =1
			uint8_t tra:1; // transmitter=1/ reciever=0
			uint8_t reserv_sr3:1; // reserved
			uint8_t gencall:1;	// generall call header=1 (slave)
			uint8_t reserv2_sr3:2; // reserved
			uint8_t dualf:1; // Dual flag OAR2=0 OAR2=1 (slave)
		};
	};
}i2cStatusReg_t;
//---------------
typedef struct
{
	// ����� ������� ��� �������
	union
	{
		struct
		{
			uint8_t i2cReserv:1;			// ������
			uint8_t busError:1;		// ������ �� ����
			i2cModeTxRx_t ModeTxRx:1;// ����� ��������
			uint8_t Busy:1; 	// ����� ������
			i2cResult_t Result:1; 				// ��������� ��������
			uint16_t adr9_0bit:11; 		// 10 ��� �����
		};
		struct
		{
			uint8_t i2cReserv2:5;			// ������
			uint8_t header; // 3 ����
			uint8_t addr; // 7 ���
		};
	};
	uint8_t*  Pointer; 
	uint8_t 	PointerSize; 
	uint8_t*	Array; 
	uint8_t 	ArraySize;
	
}i2cStack_t;

i2cStack_t i2cStack;
i2cStatusReg_t i2cReg;

//------------------
uint8_t i2cMaster7BitSendSend(uint8_t DeviceAddress, 
															void*   Pointer, 
															uint8_t PointerSize, 
															void*   Array, 
															uint8_t ArraySize)
{
	
	if (!i2cStack.Busy)
	{
		// ����� ������ ������
		//i2cStack.Flag=send;
		// ����������� ����� �������� ������ � 7������ ������
		I2C->OARH=(I2C->OARH&0x06)||0x40;
		// ���������� ����� ��� ������
		i2cStack.addr=DeviceAddress<<1;
		i2cStack.Pointer=(uint8_t*)Pointer;
		i2cStack.PointerSize=PointerSize;
		i2cStack.Array=Array;
		i2cStack.ArraySize=ArraySize;
		i2cStack.ModeTxRx=i2cTxTx;
		//i2cStack.State=i2cRun;
		i2cStack.Busy=1;
		i2cStack.Result=i2cSuccess;
		I2C->start=1;
		return 0;
	}
	return 1;
}

uint8_t i2cMaster7BitSendReceive(uint8_t DeviceAddress, 
															void*   Pointer, 
															uint8_t PointerSize, 
															void*   Array, 
															uint8_t ArraySize)
{
	
	if (!i2cStack.Busy)
	{
		// ����� ������ ������
		//i2cStack.Flag=send;
		// ����������� ����� �������� ������ � 7������ ������
		I2C->OARH=(I2C->OARH&0x06)||0x40;
		// ���������� ����� ��� ������
		if (PointerSize)
		{
			// ����� ��� ������
			i2cStack.addr=DeviceAddress<<1; 
		}
		else
		{
			// ����� ��� ������
			i2cStack.addr=DeviceAddress<<1|0x01;
		}
		i2cStack.Pointer=(uint8_t*)Pointer;
		i2cStack.PointerSize=PointerSize;
		i2cStack.Array=Array;
		i2cStack.ArraySize=ArraySize;
		i2cStack.ModeTxRx=i2cTxRx;
		//i2cStack.State=i2cRun;
		i2cStack.Busy=1;
		i2cStack.Result=i2cSuccess;
		I2C->start=1;
		return 0;
	}
	if (i2cReg.SR1==0 &&i2cReg.SR1==0&&i2cReg.SR1==0 ) 
	{
	I2C_Init_7bit(100000);
	i2cStack.Busy=0;
	}
	//i2cStack.busError=1;
			//	I2C->pe=0;
	return 1;
}


void interrupt_i2c(void)
{
	
	i2cReg.SR1=I2C->SR1;
	i2cReg.SR3=I2C->SR3;
	i2cReg.SR2=I2C->SR2;
	i2cStack.Busy=i2cReg.busy;
	// ������ �� ����
		if (i2cReg.berr)
		{
			I2C->berr=0; // ����� �����
			i2cStack.busError=1;
			//I2C_Init_7bit(100000);
			//I2C->stop=1; // ��������� STOP
			return;
		}
		
	// 
	if (i2cReg.msl)
	{
		// ���� ����� ������� � ��������� ���� �������
		if (i2cReg.sb)
		{
			
			// ���������� ������ ���������
			if (I2C->addmode)
			{
				// 10-bit address
				I2C->DR=i2cStack.header;				
			}
			else
			{
				readDataLabel: // ����� ��� �������� 10 ������� ������ 
				// 7-bit address
				I2C->DR=i2cStack.addr;
				// ���� ���� ������ ������
				if (i2cStack.addr&0x01)
				{
					// ���� ��� ������ ��� ��������, �� ��� �� ������� ����
					if (!i2cStack.ArraySize)
						I2C->stop=1;
				}
				
				return;
			}
		}
		// ���������� �����
		if (i2cReg.add10)
		{
			//I2C->DR=i2cStack.addr;
			goto readDataLabel;
			//return;
		}
		//-------
		// ����, ��� ����� �������
		// ����������� ���-�� ���� ��� �������
		if (i2cReg.addr)
		{
			if (i2cStack.addr&0x01)
			{
				if (i2cStack.ArraySize>1)
				{
					// ��������� ������ 1 ����
					I2C->ack=1;
				}
				else
				{
					// ��������� ����� 1 ���� ������
					I2C->ack=0;
					I2C->stop=1;
				}
				return;
			}
		}
		//=============================
		// ��������� ����� � ��������
		//=============================
		if (i2cReg.tra)
		{
			//=============================
		// ��������� ��������
		//=============================
			
			// ���� ������� NACK �� �����, �� ���������� STOP 
		if (i2cReg.af)
			{
				I2C->stop=1;
				i2cStack.Busy=0;
			return;
			}
			// ��������� �������� ����� ������-������
			if (i2cStack.ModeTxRx==i2cTxTx)
			{
				// �������� ������
				if (i2cStack.PointerSize)
				{
					I2C->DR=*i2cStack.Pointer;
					i2cStack.PointerSize--;
					i2cStack.Pointer++;
					return;
				}
				if (i2cStack.ArraySize)
				{
					I2C->DR=*i2cStack.Array;
					i2cStack.ArraySize--;
					i2cStack.Array++;
					return;
				}
				I2C->stop=1;
				return;
			}
			else
			{
				// ��������� �������� ����� ������-������
				// �������� ������
				if (i2cStack.PointerSize)
				{
					I2C->DR=*i2cStack.Pointer;
					i2cStack.PointerSize--;
					i2cStack.Pointer++;
					return;
				}
								
				// �������� �� ������������� ������
				if (i2cStack.ArraySize)
				{
					i2cStack.addr|=1; // ����� ������
					I2C->start=1;
				}
				else 
				{
					I2C->stop=1;
				}
			return;
			}
			
		}
		else
		{
			//=====================
			// ����� ������
			//=====================
			// ���� ������� NACK �� �����, �� ���������� STOP 
		if (i2cReg.af)
			{
				I2C->stop=1;
				i2cStack.Busy=0;
			return;
			}
			
			// ����� �������-���������
			// ����� ������
			switch (i2cStack.ArraySize)
			{
				// ��� ������ ��� ������
				
				case 1:
					*i2cStack.Array=I2C->DR;
					//I2C->stop=1;
					i2cStack.ArraySize--;
					break;
				case 2:
					I2C->ack=0;
					*i2cStack.Array=I2C->DR;
					i2cStack.ArraySize--;
					i2cStack.Array++;
					//I2C->ack=0;
					I2C->stop=1;
					break;
					
				default:
					
					*i2cStack.Array=I2C->DR;
					I2C->ack=1;
					i2cStack.ArraySize--;
					i2cStack.Array++;
			}
			
			return;
			
		}
		
	}
	else
	{
		//======================
		// Slave
		//======================
		
	}
	// ��������� 1-�� ���� � ���������
	if (i2cReg.rxne)
			{
				*i2cStack.Array=I2C->DR;
					//I2C->stop=1;
					i2cStack.ArraySize--;
				i2cStack.Busy=0;
				return;
			}
	//��������� ��������� ������	
// ��������� ������
	if (i2cReg.SR2)
	{
		// ������ ��������/������
		//i2cStack.Flag=idle;
		// ��� ������
		
		if(i2cReg.af)
		{
			//i2cStack.Busy=0;
			I2C->af=0;// ����� �����
			//I2C->stop=1; // ��������� STOP
			i2cReg.SR1=I2C->SR1;
			i2cReg.SR3=I2C->SR3;
			i2cReg.SR2=I2C->SR2;
			i2cStack.Busy=i2cReg.busy;
		}
		/*
		// ������ �� ����
		if (i2cReg.berr)
		{
			I2C->berr=0; // ����� �����
			i2cStack.busError=1;
			//I2C_Init_7bit(100000);
			//I2C->stop=1; // ��������� STOP
			return;
		}
		*/
		// ������ ��������
		if (i2cReg.ovr)
		{
			I2C->ovr=0;
			return;
		}
		// ������ ���������
		if (i2cReg.arlo)
		{
			I2C->arlo=0;
			return;
		}
		// slave in Halt mode
		if (i2cReg.wufh)
		{
			I2C->wufh=0;
			return;
		}
		
	}


	if (i2cReg.stopf||i2cReg.sb)
			{
				i2cStack.busError=1;
				I2C->pe=0;
				I2C->itevten=0;
				return;
			}
	
	// ����� ������		
	return;		
	//i2c_Task.
}
	/*
	uint8_t*  Pointer; 
	uint8_t 	PointerSize; 
	uint8_t*	Array; 
	uint8_t 	ArraySize;
	*/
// ���������� � ��������� ����������
int16_t led(uint8_t argc,char *argv[])
{
	if (argc==2)  
	{
		if (strcmp(argv[1],"on")==0) // �������� ���������
		{
			cnt_on2++;
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
			printf("��������� ������� %d\r\n",cnt_on2);
			return 0;
		}
		else 
			if (strcmp(argv[1],"off")==0)  // ��������� ���������
			{
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
				printf("��������� ��������\r\n");
				return 0;
			}
			else
			{
				return 2;
			}
	}
	else
		return 1;
}


uint8_t bl=1; // ���������� ��� ��������� � ���������� �������
// ������� ��� ��������� � ���������� �������
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc==2) 
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

const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};
// ������� ����������� �������-����� ��� ������������ ��������

const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};

//char simvol;		
//============================ ����� ������ �� UART
#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif





//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
	}
}
#endif


uint16_t nmb=0;
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

	// ������������� ��������� ������.
	
	cmdinit('\r', // ����-������
					'\n', // 2 ����-������
					testf, // ��������� �� ������� ��������-�������
					2,  	// ���-�� ������� � �������
					RetText, // ������� �����-������� ������
					2);		// ������ ������� �����-�����
	/*
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// ��������� ������ �������� �� uart2 � ��������� ������
	UART2_SetRxHandler(cmdinputchar);
	//UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// ������������� ��� ������ printf
	stdio_InitPrintf(uart2_sendtobuffer);
	UART2_Cmd(ENABLE);
	*/
I2C_Init_7bit(100000);
	enableInterrupts();
	/*
	i2cMaster7BitSendSend(0b1111000, 
															a, 
															2, 
															b, 
															4);
	*/
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	printf("Check cmdline\r\n");
	while (1)
  {
		cmdmainloop(); // ��������� ��������� ������
	//	printf("qwe5675089tr %d\r\n",nmb);
		nmb++;
		/*
		i2cMaster7BitSendSend(0b1010111, 
															a, 
															2, 
															b, 
															4);
	*/
	i2cMaster7BitSendReceive(0b1101000, 
															a, 
															1, 
															b, 
															4);
	if (i2cStack.busError) {
					i2cStack.Busy=0;
					i2cStack.busError=0;
					I2C->pe=0;
					I2C_Init_7bit(100000);
					
					}
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(100);
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
