#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>

void initServer();

void updateServer(double core_temp, double ambient_temp, double battery_level);