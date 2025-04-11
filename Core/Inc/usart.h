/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define ENABLE_YMODEM 0           //USE 1 to enable YMODEM file recieve
#define UART_BUFFER_SIZE 4096
/* USER CODE END Private defines */

void system_usart1_init(uint32_t baudrate);

/* USER CODE BEGIN Prototypes */
#if ENABLE_YMODEM == 1
  extern uint8_t rx_data;
  extern uint8_t rx_buffer[UART_BUFFER_SIZE];
  extern uint16_t rx_buffer_index;
#endif
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

