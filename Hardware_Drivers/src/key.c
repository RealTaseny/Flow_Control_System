//
// Created by Taseny on 25-4-6.
//

#include "key.h"

uint8_t key_scan(void)
{
  uint8_t key_value = 0;
  uint8_t key_buffer = 0xFF;

  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_ON_OFF_PORT, KEY_ON_OFF_PIN);
  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_CENTER_PORT, KEY_CENTER_PIN);
  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_LEFT_PORT, KEY_LEFT_PIN);
  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_DOWN_PORT, KEY_DOWN_PIN);
  key_buffer = key_buffer << 1 | HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN);
  key_buffer = ~key_buffer;
  switch (key_buffer)
  {
    case KEY_UP: key_value = KEY_UP; break;
    case KEY_DOWN: key_value = KEY_DOWN; break;
    case KEY_LEFT: key_value = KEY_LEFT; break;
    case KEY_RIGHT: key_value = KEY_RIGHT; break;
    case KEY_CENTER: key_value = KEY_CENTER; break;
    case KEY_ON_OFF: key_value = KEY_ON_OFF; break;
    default: key_value = INVALID; break;
  }
  return key_value;
}
