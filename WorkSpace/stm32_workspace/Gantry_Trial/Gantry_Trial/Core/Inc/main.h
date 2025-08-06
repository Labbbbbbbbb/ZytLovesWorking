/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define detect_Pin GPIO_PIN_2
#define detect_GPIO_Port GPIOE
#define detect_EXTI_IRQn EXTI2_IRQn
#define detectl2_Pin GPIO_PIN_3
#define detectl2_GPIO_Port GPIOE
#define detectr1_Pin GPIO_PIN_4
#define detectr1_GPIO_Port GPIOE
#define detectr2_Pin GPIO_PIN_5
#define detectr2_GPIO_Port GPIOE
#define u4_area3tx_Pin GPIO_PIN_0
#define u4_area3tx_GPIO_Port GPIOA
#define u4_area3rx_Pin GPIO_PIN_1
#define u4_area3rx_GPIO_Port GPIOA
#define u2_area1_y_tx_Pin GPIO_PIN_2
#define u2_area1_y_tx_GPIO_Port GPIOA
#define u2_area1_y_rx_Pin GPIO_PIN_3
#define u2_area1_y_rx_GPIO_Port GPIOA
#define Reset_Pin GPIO_PIN_7
#define Reset_GPIO_Port GPIOE
#define Reset1_Pin GPIO_PIN_8
#define Reset1_GPIO_Port GPIOE
#define Reset2_Pin GPIO_PIN_9
#define Reset2_GPIO_Port GPIOE
#define u3_area2_x_tx_Pin GPIO_PIN_10
#define u3_area2_x_tx_GPIO_Port GPIOB
#define u3_area2_x_rx_Pin GPIO_PIN_11
#define u3_area2_x_rx_GPIO_Port GPIOB
#define CylinderX_Pin GPIO_PIN_13
#define CylinderX_GPIO_Port GPIOB
#define ElectromagnetX_Pin GPIO_PIN_14
#define ElectromagnetX_GPIO_Port GPIOB
#define CylinderYL_Pin GPIO_PIN_12
#define CylinderYL_GPIO_Port GPIOD
#define ElectromagnetYL_Pin GPIO_PIN_13
#define ElectromagnetYL_GPIO_Port GPIOD
#define CylinderYR_Pin GPIO_PIN_14
#define CylinderYR_GPIO_Port GPIOD
#define ElectromagnetYR_Pin GPIO_PIN_15
#define ElectromagnetYR_GPIO_Port GPIOD
#define u3_area2_y_tx_Pin GPIO_PIN_6
#define u3_area2_y_tx_GPIO_Port GPIOC
#define u3_area2_y_rx_Pin GPIO_PIN_7
#define u3_area2_y_rx_GPIO_Port GPIOC
#define u1_area1_x_tx_Pin GPIO_PIN_9
#define u1_area1_x_tx_GPIO_Port GPIOA
#define u1_area1_x_rx_Pin GPIO_PIN_10
#define u1_area1_x_rx_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
