#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 25, 26, 27, 14);

byte celcius[8] = {B10000, B00111, B01000, B01000,
                   B01000, B01000, B00111, B00000};
byte flame[3][8] = {
    {B00010, B00110, B01010, B01001, B10001, B10101, B01110, B00000},
    {B01000, B00100, B00110, B01001, B10001, B10101, B01110, B00000},
    {B00100, B00100, B01010, B01001, B10001, B10101, B01110, B00000}};
byte smoke[3][8] = {
    {B00000, B01000, B00000, B00000, B00010, B00000, B01000, B00000},
    {B00100, B00000, B00000, B00010, B00000, B10000, B00000, B00000},
    {B00000, B00000, B00100, B00000, B10000, B00000, B00010, B00000}};
int frame = 0;

void lcdSetup() {
    lcd.begin(16, 2);
    lcd.createChar(0, flame[0]);
    lcd.createChar(1, flame[1]);
    lcd.createChar(2, flame[2]);
    lcd.createChar(3, smoke[0]);
    lcd.createChar(4, smoke[1]);
    lcd.createChar(5, smoke[2]);
    lcd.createChar(7, celcius);
    lcd.setCursor(1, 0);
    lcd.print("MEAT TEMP:");
    lcd.setCursor(1, 1);
    lcd.print("BBQ TEMP:");
}
void updateLcd(double cT, double aT) {
    lcd.setCursor(11, 0);
    if (cT > -20) {
        lcd.print(cT, 1);
        lcd.setCursor(15, 0);
        lcd.write(byte(7));
    } else {
        lcd.print("N/C  ");
    }

    lcd.setCursor(10, 1);
    if (aT > -20) {
        lcd.print(aT, 1);
        lcd.setCursor(15, 1);
        lcd.write(byte(7));
    } else {
        lcd.print("N/C  ");
    }

    switch (frame) {
        case 0:
            frame = 1;
            lcd.setCursor(0, 1);
            lcd.write(byte(2));
            lcd.setCursor(0, 0);
            lcd.write(byte(3));
            break;
        case 1:
            frame = 2;
            lcd.setCursor(0, 1);
            lcd.write(byte(1));
            lcd.setCursor(0, 0);
            lcd.write(byte(4));
            break;
        case 2:
            frame = 0;
            lcd.setCursor(0, 1);
            lcd.write(byte(0));
            lcd.setCursor(0, 0);
            lcd.write(byte(5));
            break;
    }
}