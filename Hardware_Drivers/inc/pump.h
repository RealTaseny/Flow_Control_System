//
// Created by Taseny on 25-4-4.
//
#pragma once
#ifndef PUMP_H
#define PUMP_H

#include "gpio.h"

#define MAX_FLOW 10000.0f
#define PWM_PERIOD 5000U
#define PUMP_PWM_CH 0x00000000U

#define FORWARD 0
#define BACKWARD 1
#define PUMP_SLEEP 0
#define PUMP_WORK 1

#define PUMP_NSLEEP_GPIO GPIOC
#define PUMP_NSLEEP_PIN GPIO_PIN_13

#define PUMP_DIR_GPIO GPIOA
#define PUMP_DIR_PIN GPIO_PIN_1

#define PUMP_NSLEEP(x) ((x) == (0) ? (HAL_GPIO_WritePin(PUMP_NSLEEP_GPIO, PUMP_NSLEEP_PIN, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(PUMP_NSLEEP_GPIO, PUMP_NSLEEP_PIN, GPIO_PIN_SET)))
#define PUMP_DIR(x) ((x) == (0) ? (HAL_GPIO_WritePin(PUMP_DIR_GPIO, PUMP_DIR_PIN, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(PUMP_DIR_GPIO, PUMP_DIR_PIN, GPIO_PIN_SET)))

void pump_init(void);
void pump_sleep_control(uint8_t status);
void set_pump_flow(int16_t flow);

extern TIM_HandleTypeDef htim5;

#endif //PUMP_H
