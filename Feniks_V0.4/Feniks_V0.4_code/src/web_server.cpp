#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>

#include "wifi_credentials.h"

const char *ssid = YOUR_SSID;
const char *password = YOUR_PASSWORD;

AsyncWebServer server(80);

void initServer() {
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    // retrieve the previous up time and append it to a log file
    File data_file = SPIFFS.open("/data", FILE_READ);
    String data = "";
    while (data_file.available()) {
        data = data + (char)data_file.read();
    }
    data_file.close();

    File log_file = SPIFFS.open("/log", FILE_APPEND);
    log_file.print("up time in ms: ");
    log_file.print(data.substring((data.lastIndexOf(",") + 1), data.length()));
    log_file.print("\n");
    log_file.close();

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html");
    });
    server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/gauge.min.js");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css");
    });
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico");
    });
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/data");
    });
    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/log");
    });
    // Start server
    server.begin();
}

void updateServer(double core_temp, double ambient_temp, double battery_level) {
    File file = SPIFFS.open("/data", FILE_WRITE);
    file.print(core_temp);
    file.print(",");
    file.print(ambient_temp);
    file.print(",");
    file.print(battery_level);
    file.print(",");
    file.print(millis());
    file.close();
}
