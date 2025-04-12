/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include <flow_counter.h>
#include <stdio.h>
#include "task.h"


#include "ips_160.h"
#include "vavle.h"
#include "bat_detect.h"
#include "key.h"

#include "menu.h"

extern void* MENU;

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t defaultTaskHandle1;
const osThreadAttr_t defaultTask_attributes1 = {
  .name = "defaultTask1",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal5,
};

void StartDefaultTask(void *argument);
void StartDefaultTask1(void *argument);

void freertos_init(void);

void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

void vApplicationIdleHook( void )
{

}

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
  printf("Stack Overflow\n");
}

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void freertos_init(void) {
  ips160_full(RGB565_BLACK);
  MENU = menu_builder();

  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  defaultTaskHandle1 = osThreadNew(StartDefaultTask1, NULL, &defaultTask_attributes1);
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartDefaultTask(void *argument)
{
  uint32_t time = 0;
  TickType_t task1_tick = xTaskGetTickCount();
  for(;;)
  {
    sprintf(display_string_buffer, "%d", time);
    //ips160_show_string(0, 48, display_string_buffer, RGB565_BLACK, RGB565_WHITE, 1);
    time++;
    vTaskDelayUntil(&task1_tick, pdMS_TO_TICKS(50));
  }
}

void StartDefaultTask1(void *argument)
{
  TickType_t task2_tick = xTaskGetTickCount();
  uint8_t key_old = 0, key_down = 0, key_value = 0;
  for(;;)
  {
    key_old = key_value;
    key_value = key_scan();
    key_down = key_value & (key_value ^ key_old);
    if (key_down) menu_key_pressed(MENU, key_value);
    vTaskDelayUntil(&task2_tick, pdMS_TO_TICKS(10));
  }
}

