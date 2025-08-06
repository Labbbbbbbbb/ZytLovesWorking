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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "User_SMS_STS.h"
#include "SCServo.h"
#include "io_retargetToUart.h"
#include "math.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DISTANCE 1000 //云台到白板的距离 单位mm
#define PI 3.141592
#define ANGLE_MAXIMUM 400  //限定舵机能往一边转的角度（4095为一圈
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
/***target from openmv***/
int16_t target_spe1;
int16_t target_spe2;
uint8_t Receivebuffer[8];
/***Feedback Data***/
int16_t fdb_pos1;
uint16_t fdb_spd1;
uint8_t fdb_acc1;
int16_t fdb_pos2;
uint16_t fdb_spd2;
uint8_t fdb_acc2;

int16_t ori_pos1;
int16_t ori_pos2;

uint16_t flag;
/***Feedback Data***/

/*pos_*/
double real_x,real_y;
double target_pos1,target_pos2;

/*spe_*/
uint16_t spe1;
uint16_t spe2;

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t mode1=1; //电机模式
  uint8_t mode2=1; //电机模式
  WheelMode(1,mode1); //模式0：位置模式 模式1：恒速； 模式2：pwm调速；模式3：步进
  WheelMode(2,mode2); //模式0：位置模式 模式1：恒速； 模式2：pwm调速；模式3：步进
   WriteSpe(1,0,0);
   WriteSpe(2,0,0);
  HAL_UART_Receive_IT(&huart2,Receivebuffer,sizeof(Receivebuffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /**位置伺服方案
    real_x=0;
    real_y=0;
    target_pos1=atan(real_x/DISTANCE);
    target_pos2=atan(real_y/DISTANCE);
    //WritePosEx(1, (int16_t)(target_pos1/2/PI*4095), 4800, 50);
    //WritePosEx(2, (int16_t)(target_pos2/2/PI*4095), 4800, 50);
    */
    fdb_pos1=ReadPos(1);
    fdb_pos2=ReadPos(2);
    fdb_spd1=ReadSpeed(1);
    fdb_spd2=ReadSpeed(2);    //反馈数据
    if(flag==0){    //只运行一次，记录初始角度数据（一般为2048上下
        ori_pos1=fdb_pos1;
        ori_pos2=fdb_pos2;
        spe2=300;
        flag=1;       
    }
    

  /*判断角度是否在正常范围内*/  
   if(ori_pos1>=ANGLE_MAXIMUM&&ori_pos1<=4095-ANGLE_MAXIMUM)
    {
      if(fdb_pos1>=ori_pos1+ANGLE_MAXIMUM||fdb_pos1<=ori_pos1-ANGLE_MAXIMUM)
      {
        spe1=0;
        mode1=0;
        WheelMode(1,mode1);
      }
    }

    if(ori_pos2>=ANGLE_MAXIMUM&&ori_pos2<=4095-ANGLE_MAXIMUM)
    {
      //if(fdb_pos2<=3000&&fdb_pos2>=2000)
      if(fdb_pos2>=ori_pos2+ANGLE_MAXIMUM-150||fdb_pos2<=ori_pos2-ANGLE_MAXIMUM+50)
      {
        spe2=0;
        mode2=0;
        WheelMode(2,mode2);
      }
    }
    /*判断角度是否在正常范围内*/ 

/*位置模式复位*/
   if(mode2==0||mode1==0)
   {
    WritePosEx(1, 2048, 1000, 50);
    WritePosEx(2, 2223, 1000, 50);
   }

    if(mode2==1){
      WriteSpe(2,spe2,50);
    }
     if(mode1==1){
      WriteSpe(1,spe1,50);
    }
     printf("%d,%d,%d,%d,%d\n",spe2,ori_pos2,fdb_pos2,ReadSpeed(1),fdb_pos1);
   //HAL_Delay(3000);
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
}

/* USER CODE BEGIN 4 */

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
