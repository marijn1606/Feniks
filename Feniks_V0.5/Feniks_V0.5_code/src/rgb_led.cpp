#include <Arduino.h>

void HSL_led(int pin, float H, float S, float L) {
    float C = (1 - abs(2 * L - 1)) * S;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = L - C / 2;
    float Rf, Gf, Bf;
    if (H >= 0 && H < 60) {
        Rf = C;
        Gf = X;
        Bf = 0;
    } else if (H >= 60 && H < 120) {
        Rf = X;
        Gf = C;
        Bf = 0;
    } else if (H >= 120 && H < 180) {
        Rf = 0;
        Gf = C;
        Bf = X;
    } else if (H >= 180 && H < 240) {
        Rf = 0;
        Gf = X;
        Bf = C;
    } else if (H >= 240 && H < 300) {
        Rf = X;
        Gf = 0;
        Bf = C;
    } else {
        Rf = C;
        Gf = 0;
        Bf = X;
    }
    neopixelWrite(pin, round((Rf + m) * 255), round((Gf + m) * 255), round((Bf + m) * 255));
}
