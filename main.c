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
//#include "main.h"
#include "stm8s.h"
#include "stm8s_it.c"
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*
bit8_t tst;
//uint8_t ask, sr1,sr2,sr3;
//test_t r;
i2c_sr1_t sr1;
i2c_sr2_t sr2;
i2c_sr3_t sr3;
*/

#define Size_Master_Buf 20



typedef struct
{
i2cFunc_t Func;
I2CEventBit_t ItEvent;
uint8_t	DeviceAddrRW;
uint8_t *ArraySend;
uint8_t NumSend;
uint8_t *ArrSendReceive; 
uint8_t NumSendReceive;
uint8_t CurrentIndex;
}i2cTask_t;

i2cTask_t i2cTask={0,0,0,0,0,0,0,0};

I2C_Event_TypeDef i2cEventGet(void)
{
	i2cTask.ItEvent.event= I2C_GetLastEvent();
	return i2cTask.ItEvent.event;
}

void i2cTaskReset(void)
{
	//i2cTask.ArraySend=ArraySend;
	i2cTask.NumSend=0;
	//i2cTask.ArrReceive=0;
	i2cTask.NumSendReceive=0;
	i2cTask.CurrentIndex=0;
}
// ������� �������� ������ArraySend ������NumSend �� ������DeviceAddress
void i2cMasterSend(uint8_t DeviceAddress,uint8_t *ArraySend, uint8_t NumSend)
{
	while(i2cTask.Func);
	i2cTaskReset(); // ����� ���������
	i2cTask.Func=i2cSend; // �������� �������
	i2cTask.DeviceAddrRW=DeviceAddress<<1; //�������� ������
	i2cTask.ArraySend=ArraySend; // �������� ��������� �� ������
	i2cTask.NumSend=NumSend; // �������� ���-�� ���� ��� ��������
	I2C_GenerateSTART();  // ��������� ������
}

// ������� �������� � ������ ������ ������ ��������, � ����� �������� ������.
void i2cMasterSendSend(uint8_t DeviceAddress, uint8_t *ArrayAddress, uint8_t NumAddress, uint8_t *ArraySend, uint8_t NumSend)
{
	while(i2cTask.Func);
	i2cTaskReset();	// ����� ���������
	i2cTask.Func=i2cSendSend; // �������� �������
	i2cTask.DeviceAddrRW=DeviceAddress<<1;//�������� ������
	i2cTask.ArraySend=ArrayAddress; // �������� ��������� �� ����� �������
	i2cTask.NumSend=NumAddress;// �������� ���-�� ���� ��� ��������
	i2cTask.ArrSendReceive=ArraySend; // �������� ��������� ������� �� ������ 
	i2cTask.NumSendReceive=NumSend; // �������� ���-�� ������ ��� ��������
	I2C_GenerateSTART();  // ��������� ������
}

// ������� ��������� ������ � ������*ArrReceive������������NumReceive �� ������DeviceAddress
void i2cMasterReceive(uint8_t DeviceAddress, uint8_t *ArrReceive, uint8_t NumReceive)
{
	while(i2cTask.Func);
	i2cTaskReset();	// ����� ���������
	i2cTask.Func=i2cReceive; // �������� �������
	i2cTask.DeviceAddrRW=DeviceAddress<<1|1;//�������� ������ �� ������
	i2cTask.ArrSendReceive=ArrReceive; // �������� ��������� ������� �� ������ 
	i2cTask.NumSendReceive=NumReceive; // �������� ���-�� ������ ��� ��������
	I2C_GenerateSTART();  // ��������� ������
}

// ������� ���������� � ������ ������ *ArrSend � ����������NumSend, ����� ��������� � ������*ArrReceive � ����������NumReceive
void i2cMasterSendReceive(uint8_t DeviceAddress, uint8_t *ArrSend, uint8_t NumSend, uint8_t *ArrReceive, uint8_t NumReceive)
{
	while(i2cTask.Func);
	i2cTaskReset();	// ����� ���������
	i2cTask.Func=i2cSendReceive; // �������� �������
	i2cTask.DeviceAddrRW=DeviceAddress<<1;//�������� ������
	i2cTask.ArraySend=ArrSend; // �������� ��������� �� ����� �������
	i2cTask.NumSend=NumSend;// �������� ���-�� ���� ��� ��������
	i2cTask.ArrSendReceive=ArrReceive; // �������� ��������� ������� �� ������ 
	i2cTask.NumSendReceive=NumReceive; // �������� ���-�� ������ ��� ��������
	I2C_GenerateSTART();  // ��������� ������
}


//uint8_t f;
void i2cSendAddress(void)
{
	
	if (i2cTask.DeviceAddrRW&0x01) 
	{
			// ���� ���������� ����� �� ������,
			// �� ������� ������� ���� ���� ���������
			switch(i2cTask.NumSendReceive)
			{
				case 0:
						// ���� � ��� ��� ���� ��� ��������, ��
						// ���������� �������� ���� � ���� ����
						//I2C_AcknowledgeConfig(I2C_ACK_NONE);
						I2C->DR=i2cTask.DeviceAddrRW;
						I2C_GenerateSTOP();
					break;
				case 1:
						// ���� ���������� ������� 1 ����, ��
						// �� ���������� ����� � ���� �� �������������
						I2C->DR=i2cTask.DeviceAddrRW;
						I2C_AcknowledgeConfig(I2C_ACK_NONE);
					break;
				default:
						// � ��������� ������� ������ ���������� �����
						// � ���� �������������
						I2C->DR=i2cTask.DeviceAddrRW;
						I2C_AcknowledgeConfig(I2C_ACK_CURR);
					break;
			}
	}
	else
	{
		// ���������� ����� �� ������
		I2C->DR=i2cTask.DeviceAddrRW;
	}
	//return i2cTask.DeviceAddrRW;
}



// �������� �� ����������� ������� �������
ErrorStatus i2cNumSendCheck(void)
{
	if (i2cTask.NumSend) return SUCCESS;
									else return ERROR;
}


i2cFunc_t i2cFuncCheck(void)
{
	return i2cTask.Func;
}

void i2cFuncSend(void)
{
	switch(i2cTask.NumSend)
	{
		case 0:
			I2C_GenerateSTOP();
			i2cTask.CurrentIndex=0;
			i2cTask.Func=i2cIdle;
			break;
		default:
		I2C->DR=i2cTask.ArraySend[i2cTask.CurrentIndex++];
		i2cTask.NumSend--;
	}
	/*
	if (i2cTask.NumSend) 
	{
	I2C->DR=i2cTask.ArraySend[i2cTask.CurrentIndex++];
	i2cTask.NumSend--;
	}
	else
	{
		i2cTask.CurrentIndex=0;
		I2C_GenerateSTOP();
	}
	*/
}


void i2cFuncSendSend(void)
{
	switch(i2cTask.NumSend)
	{
		case 0:
			// �������� ������ �� ������� �������, �.�. ������ ������
			switch(i2cTask.NumSendReceive)
			{
				case 0:
					I2C_GenerateSTOP();
					i2cTask.CurrentIndex=0;
					i2cTask.Func=i2cIdle;
					break;
				default:
					I2C->DR=i2cTask.ArrSendReceive[i2cTask.CurrentIndex++];
					i2cTask.NumSendReceive--;
					break;
			}
			break;
		default:
			// ��������� ������ ���� ���� ��� ����������
			I2C->DR=i2cTask.ArraySend[i2cTask.CurrentIndex++];
			i2cTask.NumSend--;
			if (i2cTask.NumSend==0)  i2cTask.CurrentIndex=0;
	}
	
	
	
	
	
	/*
	// ���� ���� ��� ���������� �� ������� ��� �������� ���������� 
	if (i2cTask.NumSend) 
	{
		I2C->DR=i2cTask.ArraySend[i2cTask.CurrentIndex++];
		i2cTask.NumSend--;
		// ���� ��� ��������� ���� ��� ��������, �� �������� ������� ������
		if (i2cTask.NumSend==0)  i2cTask.CurrentIndex=0;
	}
	else
	{
		if (i2cTask.NumSendReceive)
		{
			I2C->DR=i2cTask.ArrSendReceive[i2cTask.CurrentIndex++];
			i2cTask.NumSendReceive--;
		}
		else
		{
			i2cTask.CurrentIndex=0;
			I2C_GenerateSTOP();
			i2cTask.Func=i2cIdle;
		}
	}
	*/
}

void i2cFuncAddressSendReceive(void)
{
	switch(i2cTask.NumSendReceive)
		{
			case 0:
				//I2C_GenerateSTOP();
				//f=I2C->DR;
				i2cTask.Func=i2cIdle;
				//I2C_GenerateSTOP();
				break;
			
			case 1:
						//
						I2C_AcknowledgeConfig(I2C_ACK_NONE);
						I2C_GenerateSTOP();
				break;
			//case 2:
			default:
				I2C_AcknowledgeConfig(I2C_ACK_CURR);
				break;
		}
}

void i2cFuncReceive(void)
{
	
	i2cTask.ArrSendReceive[i2cTask.CurrentIndex++]=I2C->DR; // �������� ��������� ������� �� ������ 
	i2cTask.NumSendReceive--; // �������� ���-�� ������ ��� ��������
	// �������� ������� ������ 
	switch(i2cTask.NumSendReceive)
			{
				case 0:
					i2cTask.Func=i2cIdle;
					i2cTask.CurrentIndex=0;
					return;
				break;
				case 1:
						//i2cTask.Func=i2cIdle;
						I2C_AcknowledgeConfig(I2C_ACK_NONE);
						I2C_GenerateSTOP();
						//i2cTask.ArrSendReceive[i2cTask.CurrentIndex++]=i2cData; // �������� ��������� ������� �� ������ 
						//i2cTask.NumSendReceive--; // �������� ���-�� ������ ��� ��������
						
						//i2cTask.Func=i2cIdle;
						
					
					break;
				//case 1:
				//		I2C_AcknowledgeConfig(I2C_ACK_NONE);
				//	break;
				default:
						I2C_AcknowledgeConfig(I2C_ACK_CURR);
						//i2cTask.ArrSendReceive[i2cTask.CurrentIndex++]=i2cData; // �������� ��������� ������� �� ������ 
						//i2cTask.NumSendReceive--; // �������� ���-�� ������ ��� ��������
			}
	
}

void i2cFuncSendReceive(void)
{
	switch(i2cTask.NumSend)
	{
		case 0:
			i2cTask.DeviceAddrRW=i2cTask.DeviceAddrRW|1;
			I2C_GenerateSTART();
			i2cTask.CurrentIndex=0;
			//i2cTask.Func=i2cIdle;
			break;
		default:
		I2C->DR=i2cTask.ArraySend[i2cTask.CurrentIndex++];
		i2cTask.NumSend--;
	}
}

uint8_t a[10]={0xFF,2,3,4,5,6,7,8,9,10};
void Task(void)
{
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(100000, 50, 
              I2C_DUTYCYCLE_2, I2C_ACK_NONE, 
              I2C_ADDMODE_7BIT, 16 );
	I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	I2C_Cmd(ENABLE);
	i2cMasterSend(0b0111111, a, 1);
//	i2cMasterSend(0b0111111, a+5, 5);
//	i2cMasterSendSend(0b0111111, a, 1,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 2,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,0);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,1);
//	i2cMasterSendSend(0b0111111, a, 1,a+5,0);
//	i2cMasterReceive(0b0111111, a+9, 1);
// i2cMasterSendReceive(0b0111111, a, 3, a+5, 2);	

// i2cMasterSendReceive(0b0111111, a+3, 3, a+8, 2);	
	//i2cMasterReceive(0b0111111, a, 0);
	//i2cMasterSendBuf(0b0111111);	
	//i2cMasterUploadBuf(102);
	
	
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(1000);
	}
}

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
	
	//Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  while (1)
  {
  //GPIO_WriteReverse(GPIOC, GPIO_PIN_1);
	//delay_ms(1000);
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
