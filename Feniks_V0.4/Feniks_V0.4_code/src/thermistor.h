#pragma once
#include <Arduino.h>

void thermistor_setup();

double get_resistance(int adcVal);

double get_potential(int adcVal);

double get_steinhart(double R);

int sample_mcp3008(byte channel, int samples);