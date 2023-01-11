#include "Arduino.h"
#include "lcd.h"
#include "thermistor.h"
#include "web_server.h"

long timer = 0;
int interval = 750;

double core_temp;
double ambient_temp;
double battery_level;

void setup() {
    Serial.begin(115200);
    initServer();
    thermistor_setup();
    lcdSetup();
}

void loop() {
    if (millis() - timer > interval) {
        timer = millis();
        core_temp = get_steinhart(get_resistance(sample_mcp3008(0, 250)));
        ambient_temp = get_steinhart(get_resistance(sample_mcp3008(1, 250)));
        battery_level = get_potential(sample_mcp3008(2, 250));
        updateLcd(core_temp, ambient_temp);
        updateServer(core_temp, ambient_temp, battery_level);
    }
}