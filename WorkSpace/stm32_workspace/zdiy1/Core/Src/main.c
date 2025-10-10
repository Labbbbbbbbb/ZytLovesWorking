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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "io_retargetToUart.h"
#include <stdio.h>
#include "usbd_cdc_if.h"
#include "oled.h"
#include "jy901s.h"
#include "wtr_calculate.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
uint8_t tim_elapsed;
uint8_t left_ICflag;
uint8_t right_ICflag;
uint8_t captureEDGE[2];  //0==RISINGEDGE  1=FALLINGEDGE
int16_t left_cnt;  
int16_t right_cnt;
float vel_left;
float vel_right;
PID_t left_pid,right_pid;
PID_t angle_pid;
PID_t gyro_pid;
float outputright,outputleft;
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  //MPU_Init();
  SPI_PIN_Init();
  OLED_Init();
  JY901S_Init();
  char *TX = "I Love VCP\n";
  HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  pid_init(&left_pid, 3, 0.1, 0.02, 1000, 0,200);
  pid_init(&right_pid,3, 0.1, 0.02, 1000, 0,200);
  pid_init(&angle_pid,5, 0, 0.2, 80, 0,10);
  pid_init(&gyro_pid,3, 0, 0, 300, 0,20);
  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  __HAL_TIM_SetCounter(&htim3,65536/2);
  __HAL_TIM_SetCounter(&htim4,65536/2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //MPU_Data_Get();
    //printf("Pitch: %.2f°, Roll: %.2f°\n", imu_data.pitch, imu_data.roll);
    //CDC_Transmit_FS(TX, strlen(TX));

    JY901S_Update();
    // OLED_DisplayTurn(1);
    // OLED_Refresh();
    
    // OLED_ShowString(0,0,"pitch:", 16);
    // OLED_ShowString(0,30,"velocity:", 16);
    // OLED_ShowNum(50,0,(int)left_cnt,5, 16);
    // OLED_ShowNum(50,30,(int)right_cnt,5, 16);

    // OLED_Refresh();
    // printf(" %d %d\n",left_cnt,right_cnt);
    /***********ANGLE&GYRO_PID_CONTROL************/
    angle_pid.ref=0;
    angle_pid.fdb=fAngle[0];
    PID_Calc_P(&angle_pid);
   if(fAngle[0]>-4&&fAngle[0]<4)  //dead band
   {
       angle_pid.output=0;
   }
   gyro_pid.ref=angle_pid.output;
   gyro_pid.fdb=fGyro[0];
   PID_Calc_P(&gyro_pid);
      printf("angle:%.2f,gyro:%.2f\n",fAngle[0],fGyro[0]);
      printf("lcnt:%f,rcnt:%f,angle_pid:%.2f,gyro_pid:%.2f,left_pid:%.2f,right_pid:%.2f\n",vel_left,vel_right,angle_pid.output,gyro_pid.output,left_pid.output,right_pid.output);
//printf("angle:%.2f,gyro:%.2f,left:%.2f,right:%.2f\n",fAngle[0],fGyro[0],left_pid.output,right_pid.output);

    /***********VELOCITY_CALCULATE&PID_CONTROL************/
    if(tim_elapsed)
    {
      left_cnt=__HAL_TIM_GET_COUNTER(&htim3)-65536/2;
      right_cnt=__HAL_TIM_GET_COUNTER(&htim4)-65536/2;
      vel_left=left_cnt;  //rpm/28.0)*60.0*100
      vel_right=right_cnt; //rpm/28.0)*60.0*100

      left_cnt=0;
      right_cnt=0;
      //printf("vel_left:%.2f,vel_right:%.2f,%f,%f\n",vel_left,vel_right,left_pid.output,right_pid.output);
      /***********PID_CONTROL************/
      left_pid.ref=gyro_pid.output;
      right_pid.ref=gyro_pid.output;
      left_pid.fdb=vel_left;
      right_pid.fdb=vel_right;
      PID_Calc_P(&left_pid);
      PID_Calc_P(&right_pid);
      if(left_pid.output>0)
      {
        HAL_GPIO_WritePin(L_Turn_GPIO_Port, L_Turn_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, left_pid.output);
      }else
      {
        HAL_GPIO_WritePin(L_Turn_GPIO_Port, L_Turn_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000+left_pid.output);
      }
      if(right_pid.output>0)
      {
        HAL_GPIO_WritePin(R_Turn_GPIO_Port, R_Turn_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, right_pid.output);
      }else
      {
        HAL_GPIO_WritePin(R_Turn_GPIO_Port, R_Turn_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000+right_pid.output);
      }

      __HAL_TIM_SetCounter(&htim3,65536/2);
      __HAL_TIM_SetCounter(&htim4,65536/2);
		   tim_elapsed=0;

      /***********PID_CONTROL************/
    }
    
    /***********VELOCITY_CALCULATE&PID_CONTROL************/

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1)
  {
    tim_elapsed = 1;
  }
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  //inputcapture:collect the data of the difference of time
{
	// if(htim->Instance==TIM3&&htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
	// {
	// 	left_ICflag=1;
	// }
	// if(htim->Instance==TIM4&&htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
	// {
	// 	right_ICflag=1;
	// }
	

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
#ifdef USE_FULL_ASSERT
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
