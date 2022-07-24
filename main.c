/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
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
#include "stm8l15x.h"
#include "stm8l15x_it.c"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint8_t a[4]={0,0,0,0};
void main(void)
{
#ifdef  __OSA__
	OS_Init();  // Инициализация RTOS OSA
	OS_EI();   // Разрешить все прерывания
	//OS_Task_Create(7, Task1); // создаем задачу
	OS_Run(); // Запуск ядра RTOS OSA
#else
	/* Infinite loop */
	
	GPIO_Init(GPIOA, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_In_PU_No_IT);
	
	GPIO_Init(GPIOE, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);
	GPIO_Init(GPIOC, GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Fast);
	I2C_Init_7bit(100000);
  while (1)
  {
		//I2C_MasterSend(0x68, a, 3);
		I2C_MasterSendPtrReceiveData(0x68,a, 1, &a[1], 2);
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
			
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
			}
			else
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);
			}
  
	}
#endif
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
