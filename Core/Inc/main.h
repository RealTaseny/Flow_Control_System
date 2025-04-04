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
#define ON_OFF_KEY_Pin GPIO_PIN_13
#define ON_OFF_KEY_GPIO_Port GPIOC
#define POWER_CONTROL_Pin GPIO_PIN_14
#define POWER_CONTROL_GPIO_Port GPIOC
#define MOTOR_NSLEEP_Pin GPIO_PIN_15
#define MOTOR_NSLEEP_GPIO_Port GPIOC
#define MOTOR_PWM_Pin GPIO_PIN_0
#define MOTOR_PWM_GPIO_Port GPIOA
#define MOTOR_DIR_Pin GPIO_PIN_1
#define MOTOR_DIR_GPIO_Port GPIOA
#define BAT_ADC_Pin GPIO_PIN_2
#define BAT_ADC_GPIO_Port GPIOA
#define KEY_CENTER_Pin GPIO_PIN_3
#define KEY_CENTER_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define KEY_A_Pin GPIO_PIN_0
#define KEY_A_GPIO_Port GPIOB
#define KEY_B_Pin GPIO_PIN_1
#define KEY_B_GPIO_Port GPIOB
#define KEY_D_Pin GPIO_PIN_10
#define KEY_D_GPIO_Port GPIOB
#define IPS_CS_Pin GPIO_PIN_12
#define IPS_CS_GPIO_Port GPIOB
#define IPS_SCK_Pin GPIO_PIN_13
#define IPS_SCK_GPIO_Port GPIOB
#define IPS_DC_Pin GPIO_PIN_14
#define IPS_DC_GPIO_Port GPIOB
#define IPS_MOSI_Pin GPIO_PIN_15
#define IPS_MOSI_GPIO_Port GPIOB
#define IPS_BRIGHTNESS_Pin GPIO_PIN_8
#define IPS_BRIGHTNESS_GPIO_Port GPIOA
#define VAVLE_CONTROL_Pin GPIO_PIN_3
#define VAVLE_CONTROL_GPIO_Port GPIOB
#define Flowcounter_Pin GPIO_PIN_4
#define Flowcounter_GPIO_Port GPIOB
#define IPS_RES_Pin GPIO_PIN_5
#define IPS_RES_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
