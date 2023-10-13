/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_wifi_setup(String SSID) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Connect phone");
    display.print("to: ");
    display.println(SSID);
    display.println("for setup");
    display.display();
}

void display_connecting_wifi() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Connecting to WiFi...");
    display.display();
}

void display_connected_network(String SSID) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Connected:");
    display.print(SSID);
    display.display();
}

void update_oled(double Ch1, double Ch2, int battery, int RSSI) {
    display.clearDisplay();
    display.setFont();
    display.setCursor(85, 4);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.print(RSSI);
    display.print("dB");
    if (RSSI > -55) {
        display.drawCircleHelper(117, 11, 10, 2, SSD1306_WHITE);
    }
    if (RSSI > -76) {
        display.drawCircleHelper(117, 11, 7, 2, SSD1306_WHITE);
    }
    if (RSSI > -90) {
        display.drawCircleHelper(117, 11, 4, 2, SSD1306_WHITE);
    }
    if (RSSI == 0) {
        display.drawLine(116, 12, 128, 0, SSD1306_WHITE);
        display.drawLine(128, 12, 116, 0, SSD1306_WHITE);
    }
    display.drawCircle(117, 11, 1, SSD1306_WHITE);

    display.drawRect(2, 4, 15, 8, SSD1306_WHITE);
    display.drawRect(17, 6, 2, 4, SSD1306_WHITE);
    if (battery > 0) {
        display.fillRect(4, 6, 2, 4, SSD1306_WHITE);
    }
    if (battery > 25) {
        display.fillRect(7, 6, 2, 4, SSD1306_WHITE);
    }
    if (battery > 50) {
        display.fillRect(10, 6, 2, 4, SSD1306_WHITE);
    }
    if (battery > 75) {
        display.fillRect(13, 6, 2, 4, SSD1306_WHITE);
    }
    display.drawFastHLine(0, 16, 128, SSD1306_WHITE);
    display.drawFastHLine(0, 40, 128, SSD1306_WHITE);
    display.drawFastHLine(0, 63, 128, SSD1306_WHITE);

    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(0, 35);
    display.setTextSize(1);

    display.print(F("CH1: "));
    if (Ch1 >= 100) {
        display.println(Ch1, 0);
    } else {
        display.println(Ch1, 1);
    }

    display.print(F("CH2: "));
    if (Ch2 >= 100) {
        display.println(Ch2, 0);
    } else {
        display.println(Ch2, 1);
    }

    display.display();
}

bool oled_setup() {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        return false;
    }
    display.clearDisplay();
    display.fillCircle(64, 32, 16, 1);
    display.setFont();
    display.display();
    return true;
}