//
// Created by Taseny on 25-4-4.
//

#include "bat_detect.h"

#include <stdio.h>

void bat_detect_init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

float bat_detect_voltage(void)
{
    float raw_data_sum = 0;

    for (int i = 0; i < BAT_ADC_NUM; i++)
    {
        HAL_ADC_Start(&hadc1);
        if (HAL_OK == HAL_ADC_PollForConversion(&hadc1, 10)) raw_data_sum += (float)HAL_ADC_GetValue(&hadc1);
    }

    const float raw_data_avg = raw_data_sum / (float)BAT_ADC_NUM;
    bat_voltage_last = filtered_value_voltage > bat_voltage_last ? bat_voltage_last : filtered_value_voltage;
    filtered_value_voltage = alpha_voltage * FIXED_TOTAL_R * (raw_data_avg / 4096.0f * ADC_VREF / FIXED_R) + (1 - alpha_voltage) * bat_voltage_last;
    
    return filtered_value_voltage > bat_voltage_last ? bat_voltage_last : filtered_value_voltage;
}

float bat_detect_remaining(void)
{
    const float voltage = bat_detect_voltage();
    return (voltage - (float)BAT_CELL_NUM * BAT_CELL_VOLTAGE_MIN) / ((float)BAT_CELL_NUM * (BAT_CELL_VOLTAGE_MAX - BAT_CELL_VOLTAGE_MIN)) * 100.0f >= 0 ? (voltage - (float)BAT_CELL_NUM * BAT_CELL_VOLTAGE_MIN) / ((float)BAT_CELL_NUM * (BAT_CELL_VOLTAGE_MAX - BAT_CELL_VOLTAGE_MIN)) * 100.0f : 0.0f;
}
