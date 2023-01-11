#include <Arduino.h>
#include <SPI.h>

const double A = 0.7060876030e-3;
const double B = 2.154365719e-4;
const double C = 1.098444658e-7;

byte recievedbytes[2];
int adcVal;

void thermistor_setup() {
    SPI.begin();
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
}

double get_resistance(int adcVal) { return 10000.0 / (1024.0 / adcVal - 1.0); }

double get_potential(int adcVal) { return (3.3 / 1024.0 * adcVal); }

double get_steinhart(double R) {
    return 1.0 / (A + B * log(R) + C * pow(log(R), 3)) - 273.15;
}

int sample_mcp3008(byte channel, int samples) {
    adcVal = 0;
    channel = (channel << 4) + 128;
    SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
    for (int i = 0; i < samples; i++) {
        digitalWrite(SS, LOW);
        SPI.transfer(0b00000001);  // starting bit
        recievedbytes[0] = SPI.transfer(channel);
        recievedbytes[1] = SPI.transfer(0b00000000);
        adcVal += ((recievedbytes[0] & 0b00000011) << 8) + recievedbytes[1];
        digitalWrite(SS, HIGH);
    }
    SPI.endTransaction();
    adcVal /= samples;
    return (adcVal);
}