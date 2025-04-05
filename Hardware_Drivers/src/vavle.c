//
// Created by Taseny on 25-4-5.
//

#include "vavle.h"

void vavle_select(uint8_t select)
{
  if (select == VAVLE_A) HAL_GPIO_WritePin(VAVLE_GPIO_PORT, VAVLE_GPIO_PIN, GPIO_PIN_RESET);
  else if (select == VAVLE_B) HAL_GPIO_WritePin(VAVLE_GPIO_PORT, VAVLE_GPIO_PIN, GPIO_PIN_SET);
}

void vavle_turn(void)
{
  HAL_GPIO_TogglePin(VAVLE_GPIO_PORT, VAVLE_GPIO_PIN);
}
