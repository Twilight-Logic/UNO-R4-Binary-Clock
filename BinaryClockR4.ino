/*
 * Web binary clock for the Arduino UNO R4 WiFi based on the
 * UNO Q Binary Clock example by Philippe86220:
 *
 * See:
 * https://forum.arduino.cc/t/uno-q-binary-clock-webui-led-matrix-bridge-demo/1431807
 *
 * The UNO R4 WiFi has a very different architecture, but also
 * includes a matrix display. The display is slightly shorter,
 * having 12 columns to the 13 of the UNO Q, so the two pair of
 * dots separateing the two digit pais could not be accomodated.
 * Instead, the digits have been staggered to distinguish them.
 * Ticking seconds have been added along the top and bottom rows
 * which showing a single blinking led moving left to right. The
 * bottom row shows the individual seconds (0-9). The top row
 * shows the tens of seconds (0-5) starting from the halfway point.
 *
 * Conversion by John Chajecki
 * March 2026
 */


#include "MatrixTime.h"
#include "WebClock.h"

#include <RTC.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "WiFiNet.h"
#include "secrets.h"
//#include "TZdef.h"


// Set up the NTP server object
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

// Set up the matrix clock
MatrixTime matrixClock;

// Web based binary clock
WiFiServer webClock(HTTP_SERVER_PORT);
binaryWebClock binaryClock(webClock, matrixClock);


// Timing parameters in milliseconds
const uint32_t rtcUpdateInterval = 90000;   // 15 minutes
const uint16_t matrixUpdateInterval = 200;  // 5x per second
const uint16_t binclockUpdateInterval = 200;  // 5x per second


void configTimeClient(){
  timeClient.setPoolServerName(NTP_SYNC_POOL);
  timeClient.setTimeOffset(0);
  timeClient.setUpdateInterval(NTP_SYNC_INTERVAL);
}


void printCurrentTime(RTCTime timeNow){
  Serial.println("RTC time: " + String(timeNow));
}


void updateRTC() {
  auto unixTime = timeClient.getEpochTime();
  RTCTime rtctime;
  rtctime = RTCTime(unixTime);
  RTC.setTime(rtctime);
}


void updateRTCperiodic(){

  static unsigned long rtcMillis = millis();

  if (millis() - rtcMillis > rtcUpdateInterval) {
    updateRTC();
    rtcMillis = millis();
    Serial.println(F("Updated RTC."));
  }

}


void updateMatrixClock(){

  static unsigned long matrixMillis = millis();

  if (millis() - matrixMillis > matrixUpdateInterval) {
    matrixClock.clearMatrix();
    matrixClock.updateTime();
    matrixMillis = millis();
  }

}


void setup(){

      // Set the time zone
//  setenv("TZ" NET_TIME_ZONE, 1);

  // Start serial
  Serial.begin(115200);
  delay(500);

//  tzset();

  Serial.println(F("RTC initialised."));

  // Connect to WiFi
  if (WiFi_Check() != WL_NO_MODULE) {
    if (WiFi_Init(SECRET_SSID, SECRET_PASS) == WL_CONNECTED) {
      WiFi_PrintAddrs();
      WiFi_PrintCurrentNet();
    }
  }

  Serial.println(F("WiFi started."));

  // Start the NTP client
  configTimeClient();
  timeClient.begin();
  timeClient.update();
  Serial.println(F("NTP started."));

  // Start and update the RTC
  RTC.begin();
  updateRTC();
  Serial.println(F("RTC updated."));

  // Start the matrix clock
  matrixClock.begin();
  Serial.println(F("Matrix initialised."));

  binaryClock.start();

  Serial.println(F("Setup complete!"));

  Serial.flush();

}


void loop(){

  updateRTCperiodic();

  updateMatrixClock();

  if (binaryClock.clientDetected()) binaryClock.clientRespond();

  delay(10);
}

