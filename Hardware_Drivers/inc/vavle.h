//
// Created by Taseny on 25-4-5.
//

#ifndef VAVLE_H
#define VAVLE_H

#include "gpio.h"

#define VAVLE_GPIO_PORT GPIOB
#define VAVLE_GPIO_PIN GPIO_PIN_3

#define VAVLE_A 0
#define VAVLE_B 1

void vavle_select(uint8_t select);
void vavle_turn(void);

#endif //VAVLE_H
