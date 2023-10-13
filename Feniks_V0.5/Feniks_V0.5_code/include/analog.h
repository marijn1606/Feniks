#pragma once
#include <Arduino.h>

#define NTC_CH1 GPIO_NUM_0
#define NTC_CH2 GPIO_NUM_1
#define NTC_CH3 GPIO_NUM_3

double get_temp(uint8_t channel);
int get_battery_level();