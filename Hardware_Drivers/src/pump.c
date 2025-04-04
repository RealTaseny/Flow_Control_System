//
// Created by Taseny on 25-4-4.
//

#include "pump.h"

TIM_HandleTypeDef htim5;

void pump_init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    htim5.Instance = TIM5;
    htim5.Init.Prescaler = 0;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 4999;
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim5.Instance==TIM1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = IPS_BRIGHTNESS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(IPS_BRIGHTNESS_GPIO_Port, &GPIO_InitStruct);
    }
    else if(htim5.Instance==TIM5)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = MOTOR_PWM_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
        HAL_GPIO_Init(MOTOR_PWM_GPIO_Port, &GPIO_InitStruct);
    }
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, 0);
}

void set_pump_flow(int16_t flow)
{
    if (flow >= 0)
    {
        __HAL_TIM_SetCompare(&htim5, PUMP_PWM_CH, (uint32_t)(((float)flow / MAX_FLOW) * PWM_PERIOD));
        PUMP_DIR(FORWARD);
    }
    else
    {
        flow = -flow;
        __HAL_TIM_SetCompare(&htim5, PUMP_PWM_CH, (uint32_t)(((float)flow / MAX_FLOW) * PWM_PERIOD));
        PUMP_DIR(BACKWARD);
    }
}
