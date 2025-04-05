//
// Created by Taseny on 25-4-4.
//

#ifndef BAT_DETECT_H
#define BAT_DETECT_H

#include "adc.h"

#define BAT_CELL_NUM 3
#define BAT_CELL_VOLTAGE_MAX 4.2f
#define BAT_CELL_VOLTAGE_MIN 3.7f

#define BAT_ADC_NUM 128
#define FIXED_TOTAL_R (101.3f)
#define FIXED_R (9.21f)
#define ADC_VREF (3.32f)


void bat_detect_init(void);
void bat_detect_voltage(void);
void bat_detect_remaining(void);

extern float bat_voltage;
extern float bat_remaining;

#endif //BAT_DETECT_H
