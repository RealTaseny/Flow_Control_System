/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <unistd.h>
#include <string.h>
#include "ymodem.h"

#if ENABLE_YMODEM == 1
uint8_t rx_data;
uint8_t rx_buffer[UART_BUFFER_SIZE];
uint16_t rx_buffer_index;
#endif
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void system_usart1_init(uint32_t baudrate)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = baudrate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
int _write(int file, char *ptr, int len) {
  (void)file;
  HAL_UART_Transmit(&huart1, (const uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}


/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
#if ENABLE_YMODEM == 1

  packet_revieved = 0;
  if (rx_buffer_index < UART_BUFFER_SIZE - 1)
  {
    if (!packet_start && rx_data == EOT)
    {
      packet_revieved = 1;
      ymodem_receive_packet[0] = rx_data;
    }

    if (!packet_start && (rx_data == SOH || rx_data == STX))
    {
      packet_start = 1;
    }

    if (packet_start)
    {
      rx_buffer[rx_buffer_index] = rx_data;
      rx_buffer_index++;
      if (!packet_revieved)
      {
        if (rx_buffer[0] == SOH && rx_buffer_index == PACKET_SIZE_128 + PACKET_OVERHEAD)
        {
          packet_start = 0;
          packet_revieved = 1;
          memcpy(ymodem_receive_packet, rx_buffer, PACKET_SIZE_128 + PACKET_OVERHEAD);
          rx_buffer_index = 0;
          memset(rx_buffer, 0, sizeof(rx_buffer));
        }

        if (rx_buffer[0] == STX && rx_buffer_index == PACKET_SIZE_1024 + PACKET_OVERHEAD)
        {
          packet_start = 0;
          packet_revieved = 1;
          memcpy(ymodem_receive_packet, rx_buffer, PACKET_SIZE_1024 + PACKET_OVERHEAD);
          rx_buffer_index = 0;
          memset(rx_buffer, 0, sizeof(rx_buffer));
        }
      }
    }
  }

  else
  {
    printf("\r\nERROR:Get the Max UART rx buffer!\r\n");
    rx_buffer_index = 0;
    memset(rx_buffer, 0, sizeof(rx_buffer));
  }
  HAL_UART_Receive_IT(&huart1, &rx_data, 1);
#endif
  /* USER CODE END USART1_IRQn 1 */
}
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/* USER CODE END 1 */
