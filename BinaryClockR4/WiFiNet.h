#ifndef WIFINET_H
#define WIFINET_H

#include <Arduino.h>
#include <WiFiS3.h>
#include "secrets.h"

int WiFi_Check();
int WiFi_Init(const char * ssid, const char * passwd);
void WiFi_PrintAddrs();
void WiFi_PrintCurrentNet();
void printMacAddress(byte mac[]);

void flashLed(uint8_t blinks, uint32_t interval);


#endif // WIFINET