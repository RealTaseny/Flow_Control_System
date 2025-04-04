//
// Created by Taseny on 25-4-4.
//

#ifndef PUMP_H
#define PUMP_H

#include "gpio.h"

#define MAX_FLOW 10000.0f
#define PWM_PERIOD 5000U
#define PUMP_PWM_CH 0x00000000U

#define FORWARD 0
#define BACKWARD 1

#define PUMP_NSLEEP_GPIO GPIOC
#define PUMP_NSLEEP_PIN GPIO_Pin_15

#define PUMP_DIR_GPIO GPIOA
#define PUMP_DIR_PIN GPIO_Pin_1

#define PUMP_NSLEEP(x) ((x) == (0) ? (HAL_GPIO_WritePin(PUMP_NSLEEP_GPIO, PUMP_NSLEEP_PIN, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(PUMP_NSLEEP_GPIO, PUMP_NSLEEP_PIN, GPIO_PIN_SET)))
#define PUMP_DIR(x) ((x) == (0) ? (HAL_GPIO_WritePin(PUMP_DIR_GPIO, PUMP_DIR_PIN, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(PUMP_DIR_GPIO, PUMP_DIR_PIN, GPIO_PIN_SET)))

extern TIM_HandleTypeDef htim5;

#endif //PUMP_H
