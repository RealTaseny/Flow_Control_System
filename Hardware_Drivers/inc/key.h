//
// Created by Taseny on 25-4-6.
//
#pragma once
#ifndef KEY_H
#define KEY_H

#include "gpio.h"

enum KEY_LIST
{
    INVALID,
    KEY_UP = 0x01,
    KEY_DOWN = 0x02,
    KEY_LEFT = 0x04,
    KEY_RIGHT = 0x8,
    KEY_CENTER = 0x10,
    KEY_ON_OFF = 0x20
};

#define KEY_UP_PORT GPIOB
#define KEY_DOWN_PORT GPIOB
#define KEY_LEFT_PORT GPIOB
#define KEY_RIGHT_PORT GPIOB
#define KEY_CENTER_PORT GPIOA
#define KEY_ON_OFF_PORT GPIOC

#define KEY_UP_PIN GPIO_PIN_2
#define KEY_DOWN_PIN GPIO_PIN_1
#define KEY_LEFT_PIN GPIO_PIN_10
#define KEY_RIGHT_PIN GPIO_PIN_0
#define KEY_CENTER_PIN GPIO_PIN_3
#define KEY_ON_OFF_PIN GPIO_PIN_15

uint8_t key_scan(void);

#endif //KEY_H
