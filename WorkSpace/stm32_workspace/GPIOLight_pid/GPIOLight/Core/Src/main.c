/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_2.8_lcd.h"
#include "wtr_calculate.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define PhaseA 1
#define PhaseB 2
#define PhaseC 3
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t hall_a;
uint8_t hall_b;
uint8_t hall_c;
uint16_t Pwm_A;
uint16_t Pwm_B;
uint16_t Pwm_C;
uint16_t hall_cnt;
PID_t BLDC_pid;





float velocity;
void PhaseDetect(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==GPIO_PIN_SET)
	{
		hall_c=1;
	}else{
		hall_c=0;
	}
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==GPIO_PIN_SET)
	{
		hall_b=1;
	}else{
		hall_b=0;
	}
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==GPIO_PIN_SET)
	{
		hall_a=1;
	}else{
		hall_a=0;
	}
}

void SetChannel(uint8_t phase,uint8_t status)
{
	if (phase==PhaseA)
	{
		if (status==1)
		{
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,Pwm_A);
		}
		else
		{
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_1);
		}
	}
	
	if (phase==PhaseB)
	{
		if (status==1)
		{
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,Pwm_B);
		}
		else
		{
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_2);
		}
	}
	
	if (phase==PhaseC)
	{
		if (status==1)
		{
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,Pwm_C);
		}
		else
		{
			HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_3);
		}
	}
}

void SetGPIO(uint8_t phase,GPIO_PinState status)
{
	if(phase==PhaseA)
	{
		HAL_GPIO_WritePin(UL_GPIO_Port,UL_Pin,status);
	}
	if(phase==PhaseB)
	{
		HAL_GPIO_WritePin(VL_GPIO_Port,VL_Pin,status);
	}
	if(phase==PhaseC)
	{
		HAL_GPIO_WritePin(WL_GPIO_Port,WL_Pin,status);
	}
}

void Motor_Rotation_CW(void)
{
	if(hall_a==1&&hall_b==0&&hall_c==1)
	{
		SetChannel(PhaseA,1);
		SetChannel(PhaseB,0);
		SetChannel(PhaseC,0);
		SetGPIO(PhaseA,0);
		SetGPIO(PhaseB,1);
		SetGPIO(PhaseC,0);
	}
	if(hall_a==1&&hall_b==0&&hall_c==0)
	{
		SetChannel(PhaseA,1);
		SetChannel(PhaseB,0);
		SetChannel(PhaseC,0);
		SetGPIO(PhaseA,0);
		SetGPIO(PhaseB,0);
		SetGPIO(PhaseC,1);
	}
	if(hall_a==1&&hall_b==1&&hall_c==0)
	{
		SetChannel(PhaseA,0);
		SetChannel(PhaseB,1);
		SetChannel(PhaseC,0);
		SetGPIO(PhaseA,0);
		SetGPIO(PhaseB,0);
		SetGPIO(PhaseC,1);
	}
	if(hall_a==0&&hall_b==1&&hall_c==0)
	{
		SetChannel(PhaseA,0);
		SetChannel(PhaseB,1);
		SetChannel(PhaseC,0);
		SetGPIO(PhaseA,1);
		SetGPIO(PhaseB,0);
		SetGPIO(PhaseC,0);
	}
	if(hall_a==0&&hall_b==1&&hall_c==1)
	{
		SetChannel(PhaseA,0);
		SetChannel(PhaseB,0);
		SetChannel(PhaseC,1);
		SetGPIO(PhaseA,1);
		SetGPIO(PhaseB,0);
		SetGPIO(PhaseC,0);
	}
	if(hall_a==0&&hall_b==0&&hall_c==1)
	{
		SetChannel(PhaseA,0);
		SetChannel(PhaseB,0);
		SetChannel(PhaseC,1);
		SetGPIO(PhaseA,0);
		SetGPIO(PhaseB,1);
		SetGPIO(PhaseC,0);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
	STM32_LCD_Init();
	LCD_Clear(BackColor);
	
	uint16_t ADC_Value=0;
	
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start(&htim8);
	Pwm_A=4000;
	Pwm_B=4000;
	Pwm_C=4000;
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	
	PhaseDetect();
	Motor_Rotation_CW();
	HAL_Delay(7);
	PhaseDetect();
	Motor_Rotation_CW();
	
	pid_init(&BLDC_pid,1,0,0,4000,0,2000);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
			//__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,Pwm_A);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,50);
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC))
		{
			ADC_Value=HAL_ADC_GetValue(&hadc1);
			LCD_DisplayStringLine(Line0,"AD test:");
			LCD_Draw_NUM(70,300,ADC_Value);
			
			
		}
		//Pwm_A=ADC_Value*2;
		//Pwm_B=ADC_Value*2;
		//Pwm_C=ADC_Value*2;
		HAL_Delay(100);
		LCD_Draw_NUM(100,300,hall_a);
		LCD_Draw_NUM(100,400,hall_b);
		LCD_Draw_NUM(100,500,hall_c);
		LCD_Draw_NUM(130,300,velocity);
		LCD_Draw_NUM(160,300,Pwm_A);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
//{
//	HAL_UART_Transmit(&huart1,aRxBuffer,1,0);
//	HAL_UART_Receive_IT(&huart1,aRxBuffer,1);
//}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	PhaseDetect();
	Motor_Rotation_CW();
	hall_cnt++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		velocity=hall_cnt/3/4*60*1000;//rpmֵ
		BLDC_pid.ref=ADC_Value;
		BLDC_pid.fdb=velocity;
		PID_Calc_P(&BLDC_pid);
		Pwm_A=BLDC_pid.output;
		Pwm_B=BLDC_pid.output;
		Pwm_C=BLDC_pid.output;
		
		hall_cnt=0;
	}

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
