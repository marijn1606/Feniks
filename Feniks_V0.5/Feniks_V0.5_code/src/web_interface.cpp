#include <EEPROM.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
// #include "wifi_credentials.h"

String ssid;
String password;

bool setup_complete;

String host_name = "bbq";

String data = "0,0,0";
String my_ip_adress;

AsyncWebServer server(80);

void saveStringToEEPROM(int addressOffset, String stringToSave) {
    int strLength = stringToSave.length();
    EEPROM.write(addressOffset, strLength);
    char charArray[strLength + 1];
    stringToSave.toCharArray(charArray, strLength + 1);
    for (int i = 0; i < strLength; i++) {
        EEPROM.write(addressOffset + 1 + i, charArray[i]);
    }
    EEPROM.commit();
}

String readStringFromEEPROM(int addressOffset) {
    int strLength = EEPROM.read(addressOffset);
    char charArray[strLength + 1];
    for (int i = 0; i < strLength; i++) {
        charArray[i] = EEPROM.read(addressOffset + 1 + i);
    }
    charArray[strLength] = '\0';
    return String(charArray);
}

String setup_wifi_network() {
    EEPROM.begin(512);

    setup_complete = false;
    SPIFFS.begin(true);
    WiFi.hostname(host_name.c_str());
    WiFi.softAP("Feniks_setup", "");
    neopixelWrite(10, 100, 100, 100);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        neopixelWrite(10, 100, 0, 0);
        request->send(SPIFFS, "/form.html");
    });
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        neopixelWrite(10, 0, 100, 10);
        if (request->hasParam("ssid"), true) {
            inputMessage = request->getParam("ssid", true)->value();
            ssid = inputMessage;
        }
        if (request->hasParam("password"), true) {
            inputMessage = request->getParam("password", true)->value();
            password = inputMessage;
        }
        request->send(200, "text/html", "<h1>The values entered by you have been successfully sent to the device</h1>");
        setup_complete = true;
    });
    server.begin();
    while (setup_complete == false) {
        delay(100);
    }
    neopixelWrite(10, 100, 100, 0);
    WiFi.softAPdisconnect();
    saveStringToEEPROM(0, ssid);
    saveStringToEEPROM(64, password);
    return ssid;
}

String server_setup() {
    EEPROM.begin(512);

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        return "Error";
    }
    ssid = readStringFromEEPROM(0);
    password = readStringFromEEPROM(64);
    // Connect to Wi-Fi
    WiFi.hostname(host_name.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html");
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css");
    });

    // Route to load style.css file
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico");
    });

    // Route to load style.css file
    server.on("/temp_gauges.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/temp_gauges.js");
    });

    // Route to load style.css file
    server.on("/echarts.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/echarts.js");
    });

    // Route to set GPIO to HIGH
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(data));
    });
    // Start server
    server.begin();
    return WiFi.SSID();
}

int update_Server_Data(double meat, double ambient, int battery) {
    // if ((WiFi.status() != WL_CONNECTED)) {
    //     WiFi.reconnect();
    // }
    data = String(meat, 2) + "," + String(ambient, 1) + "," + String(battery);
    return WiFi.RSSI();
}