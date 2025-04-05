//
// Created by Taseny on 25-4-2.
//

#ifndef PERIPHERAL_INIT_H
#define PERIPHERAL_INIT_H

#define POWER_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET)
#define POWER_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET)

void peripheral_init(void);

#endif //PERIPHERAL_INIT_H
