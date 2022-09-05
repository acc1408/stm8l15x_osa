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
// состояние установки
typedef enum
{
	initDev=0,
	workDev=1
}stateDev_t;

typedef enum
{
	operation=0,
	load=1
}motion_t;



typedef struct
{
	// место под источники
	uint8_t seat_1:1; // Пустое место
	uint8_t seat_2:1;
	uint8_t seat_3:1;
	uint8_t seat_4:1;
	// Состояния штанги
	uint8_t levelHighest:1;
	uint8_t levelHigh:1;
	uint8_t levelLow:1;
	uint8_t levelLowest:1;
	// кнопки
	
	// настройка взаимодействия
	//motion_t motion;
	// состояние работы
	//state_t state;
	// 
}repository_t;
/* Private define ------------------------------------------------------------*/
// Управление шаговым двигателем
//Двигатель для управления вращения барабаном
#define DIR_R GPIOA, GPIO_Pin_2
#define EN_R 	GPIOA, GPIO_Pin_3
#define CLK_R GPIOA, GPIO_Pin_4
// Двигатель для управления штангой
#define DIR_L GPIOA, GPIO_Pin_5
#define EN_L 	GPIOA, GPIO_Pin_6
#define CLK_L GPIOA, GPIO_Pin_7
// Кнопки выбора барабана
#define B_B0 GPIOD, GPIO_Pin_3
#define B_B1 GPIOB, GPIO_Pin_0
#define B_B2 GPIOB, GPIO_Pin_1
#define B_B3 GPIOB, GPIO_Pin_2
// Цанга кнопка
#define B_CAN 	GPIOB, GPIO_Pin_3
// Кнопки подъема/опускания
#define B_UP 		GPIOB, GPIO_Pin_4
#define B_DOWN 	GPIOB, GPIO_Pin_5
// Кнопка ЛИМИТ ФОНА
#define B_FON GPIOD, GPIO_Pin_0
// Кнопка стоп двигателям
#define STOP GPIOD, GPIO_Pin_1
// Кнопка открытия двери
#define B_DOOR_LIM GPIOE, GPIO_Pin_5
// Режим работы работ или загрузка
#define WRK GPIOD, GPIO_Pin_2
// Опрос концевиков
// концевики штанги
#define L0_m GPIOD, GPIO_Pin_4
#define L1_m GPIOD, GPIO_Pin_5
#define L2_m GPIOD, GPIO_Pin_6
#define L3_m GPIOD, GPIO_Pin_7
//Концевики барабана
#define S0_m GPIOC, GPIO_Pin_6
#define S1_m GPIOC, GPIO_Pin_7
#define S2_m GPIOE, GPIO_Pin_6
#define S3_m GPIOE, GPIO_Pin_7
//Включение концевика
#define CAN GPIOE, GPIO_Pin_4
//Включение замка двери
#define DOOR GPIOE, GPIO_Pin_5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
stateDev_t DevStat=initDev;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#ifdef  __OSA__
void Task1(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
	
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
		OS_Delay(200);
	}
}


void PollingSignal(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
	
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
		OS_Delay(200);
	}
}


void LedBlink(void)
{
	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
		OS_Delay(200);
	}
}

#endif


//#endif
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
	TIM4_TimerOSA(1000); //Настраиваем прерывание 500мкс
	OS_EI();   // Разрешить все прерывания
	OS_Task_Create(7, Task1); // создаем задачу
	OS_Task_Create(7, PollingSignal); // создаем задачу
	OS_Task_Create(7, LedBlink); // создаем задачу
	
	OS_Run(); // Запуск ядра RTOS OSA
#else
	/* Infinite loop */
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	
	//GPIO_Init(GPIOA, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_In_PU_No_IT);
	// Настройка Драйвера моторов
	GPIO_Init(DIR_R, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(EN_R, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(CLK_R, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(DIR_L, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(EN_L, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(CLK_L, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(CAN, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(DOOR, GPIO_Mode_Out_PP_High_Fast);
	// Настройка кнопок
	GPIO_Init(B_B0, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(B_B1, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(B_B2, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(B_B3, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(B_UP, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(B_DOWN, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(L0_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(L1_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(L2_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(L3_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(S0_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(S1_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(S2_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(S3_m, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(WRK, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(STOP, GPIO_Mode_In_FL_No_IT);
	
	//CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
	I2C_Init_7bit(100000);
  while (1)
  {
		switch(DevStat)
		{
			case initDev:
			
			break;
			case workDev:
			break;
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
