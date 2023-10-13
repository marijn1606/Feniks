#include <Arduino.h>
#include <analog.h>
#include <oled.h>
#include <rgb_led.h>
#include <web_interface.h>

#define loop_interval 250
int loop_counter = 0;
double meat;
double ambient;
int battery;
int RSSI;
int hue = 0;

long last_loop_time = 0;

void setup() {
    String ssid;
    Serial.begin(115200);
    oled_setup();
    delay(100);
    neopixelWrite(10, 0, 0, 0);

    while (millis() < 2000) {
        if (digitalRead(GPIO_NUM_9) == LOW) {
            display_wifi_setup("bbq_setup");
            ssid = setup_wifi_network();
        }
    }

    display_connecting_wifi();
    ssid = server_setup();
    display_connected_network(ssid);

    delay(2000);
    meat = get_temp(NTC_CH1);
    ambient = get_temp(NTC_CH2);
    battery = get_battery_level();
}

void loop() {
    while (millis() - last_loop_time < loop_interval) {
    }
    last_loop_time = millis();
    loop_counter++;

    if ((loop_counter % 40) == 0) {
        battery = get_battery_level();
        loop_counter = 0;
    }

    meat = get_temp(NTC_CH1);
    ambient = get_temp(NTC_CH2);
    RSSI = update_Server_Data(meat, ambient, battery);
    update_oled(meat, ambient, battery, RSSI);

    // ReGuBu
    HSL_led(10, hue, 1, 0.05);
    hue > 360 ? hue = 0 : hue += 2;
}
