//
// Created by Taseny on 25-4-2.
//

#include "peripheral_init.h"

#include "usart.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"

#include "bat_detect.h"
#include "ips_160.h"
#include "pump.h"
#include "vavle.h"
#include "flow_counter.h"

#include "menu.h"

void* MENU;

void peripheral_init(void)
{
    system_gpio_init();

    POWER_ON();

    system_usart1_init(921600);
    setvbuf(stdout, NULL, _IONBF, 0);

    system_dma_init();
    system_spi1_init();
    system_spi2_init();

    ips_pwm_init();
    bat_detect_init();

    ips160_init();
    ips160_set_brightness(100);

    flow_counter_init();
    pump_init();
    pump_sleep_control(PUMP_WORK);
    set_pump_flow(0);

    vavle_select(VAVLE_B);

#if ENABLE_YMODEM == 1
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);
#endif
}