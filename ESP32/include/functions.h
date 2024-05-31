#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Deklaration der Funktionsprototypen
float readTemperature(DeviceAddress address);
void readTime();
int bcdToDec(int val);
byte decToBcd(byte val);
void setDS1307DateTime(int day, int month, int year, int hour, int minute, int second);
void callFunction();
void getSettings();
void getAnalogData();
void getTemperatureData();
void setLuefter();
void getNestStatus();
void setLicht();
void setDoor(int direction);
void insertSettings(void);

#endif
