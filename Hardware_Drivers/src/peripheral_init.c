//
// Created by Taseny on 25-4-2.
//

#include "peripheral_init.h"
#include "w25qxx_flash.h"
#include "ips_160.h"
#include "usart.h"
#include "spi.h"
#include "gpio.h"

void peripheral_init(void)
{
    //system_gpio_init();

    //system_usart1_init(921600);
    //system_usart2_init(921600);

    //system_dma_init();
    //system_spi1_init();
    //system_spi2_init();

    ips160_init();
    ips160_set_brightness(20);

    //HAL_UART_Receive_IT(&huart1, &rx_data, 1);

}