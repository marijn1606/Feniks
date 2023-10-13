#include <Arduino.h>

#include "esp_adc_cal.h"

#define NTC_CH1 GPIO_NUM_0
#define NTC_CH2 GPIO_NUM_1
#define NTC_CH3 GPIO_NUM_3

const double A = 0.7060876030e-3;
const double B = 2.154365719e-4;
const double C = 1.098444658e-7;
const double devider_resistor = 51000;

byte recievedbytes[2];
double sample_adc(uint8_t channel, int n_samples) {
    double val = 0;
    for (size_t i = 0; i < n_samples; i++) {
        val += analogReadMilliVolts(channel);
    }
    return val / n_samples;
}

double get_resistance(int measured_milivolts) {
    return devider_resistor / ((3300.0 / double(measured_milivolts)) - 1.0);
}

double get_steinhart(double R) {
    return 1.0 / (A + B * log(R) + C * pow(log(R), 3)) - 273.15;
}

double get_temp(uint8_t channel) {
    return get_steinhart(get_resistance(sample_adc(channel, 500)));
}

int get_battery_level() {
    double v_bat = sample_adc(GPIO_NUM_4, 1000);
    int level = int((v_bat - 1500.0) / 9.0);
    return level;
}