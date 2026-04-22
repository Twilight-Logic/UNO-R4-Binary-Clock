#include "WiFiNet.h"

int WiFi_Check(){

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Check WiFi module connectivity
  if (WiFi.status() == WL_NO_MODULE) {
    // Don't continue, flash led rapidly
    Serial.println(F("WiFi module unavailable!"));
    while (true){
      flashLed(255, 200);
    };
  }

  return WiFi.status();

}


int WiFi_Init(const char * ssid, const char * passwd){
  uint8_t cnt = 0;
  int wstatus = WiFi.status();
  while ( (wstatus != WL_CONNECTED) && (cnt < 11) ) {
    wstatus = WiFi.begin(ssid, passwd);
    flashLed(1, 1000);
//    delay(1000);
  }

  return wstatus;
}


void WiFi_PrintAddrs() {
  // print the board IP address:
  Serial.print(F("IP Address:\t\t"));
  Serial.println(WiFi.localIP());


  // print the board MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print(F("MAC address:\t\t"));
  printMacAddress(mac);
}


void WiFi_PrintCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID:\t\t\t"));
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print(F("BSSID:\t\t\t"));
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("Signal (RSSI):\t\t"));
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print(F("Encryption Type:\t"));
  Serial.println(encryption, HEX);
  Serial.println();
}


void printMacAddress(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}


void flashLed(uint8_t blinks, uint32_t interval){
  uint32_t timenow = millis();
  uint8_t cnt = 0;
  while(cnt < (blinks*2)) {
    if (millis() > timenow + interval) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      timenow = millis();
      cnt++;
    }
  }
}