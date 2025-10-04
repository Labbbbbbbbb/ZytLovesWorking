/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ASR_PA2_Pin GPIO_PIN_2
#define ASR_PA2_GPIO_Port GPIOA
#define ASR_PA2_EXTI_IRQn EXTI2_IRQn
#define L_Turn_Pin GPIO_PIN_3
#define L_Turn_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI4_IRQn
#define SW2_Pin GPIO_PIN_6
#define SW2_GPIO_Port GPIOA
#define SW2_EXTI_IRQn EXTI9_5_IRQn
#define ASR_PA0_Pin GPIO_PIN_0
#define ASR_PA0_GPIO_Port GPIOB
#define ASR_PA0_EXTI_IRQn EXTI0_IRQn
#define ASR_PA1_Pin GPIO_PIN_1
#define ASR_PA1_GPIO_Port GPIOB
#define ASR_PA1_EXTI_IRQn EXTI1_IRQn
#define OLED_RES_Pin GPIO_PIN_12
#define OLED_RES_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_13
#define OLED_DC_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_14
#define OLED_CS_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_15
#define SW3_GPIO_Port GPIOB
#define SW3_EXTI_IRQn EXTI15_10_IRQn
#define DHT_Pin GPIO_PIN_8
#define DHT_GPIO_Port GPIOA
#define R_Turn_Pin GPIO_PIN_15
#define R_Turn_GPIO_Port GPIOA
#define ASR_PA3_Pin GPIO_PIN_3
#define ASR_PA3_GPIO_Port GPIOB
#define ASR_PA3_EXTI_IRQn EXTI3_IRQn
#define Encoder_LA_Pin GPIO_PIN_4
#define Encoder_LA_GPIO_Port GPIOB
#define Encoder_RB_Pin GPIO_PIN_9
#define Encoder_RB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
