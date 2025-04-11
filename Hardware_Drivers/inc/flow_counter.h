//
// Created by Taseny on 25-4-4.
//
#pragma once
#ifndef FLOW_COUNTER_H
#define FLOW_COUNTER_H

#include "tim.h"

#define FLOW_COUNT_PER_L 2286.0f

void flow_counter_init(void);
float get_flow_speed(uint16_t get_times_ms);

extern uint32_t flow_counter;

#endif //FLOW_COUNTER_H
