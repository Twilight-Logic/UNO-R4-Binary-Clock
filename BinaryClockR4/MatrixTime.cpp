#include <Arduino.h>
#include "MatrixTime.h"


// Matrix geometry
const int MATRIX_WIDTH  = 12;
const int MATRIX_HEIGHT = 8;

// 3x5 font for the numbers 0 to 9
// Each line consists of 3 bits (b2 b1 b0), 1 = LED on, 0 = LED off.
const uint8_t DIGITS[10][5] = {
    // 0
    { 0b111, 0b101, 0b101, 0b101, 0b111 }, 
    // 1
    { 0b001, 0b001, 0b001, 0b001, 0b001 },
    // 2
    { 0b111, 0b001, 0b111, 0b100, 0b111 },
    // 3
    { 0b111, 0b001, 0b111, 0b001, 0b111 },
    // 4
    { 0b101, 0b101, 0b111, 0b001, 0b001 },
    // 5
    { 0b111, 0b100, 0b111, 0b001, 0b111 },
    // 6
    { 0b111, 0b100, 0b111, 0b101, 0b111 },
    // 7
    { 0b111, 0b001, 0b001, 0b001, 0b001 },
    // 8
    { 0b111, 0b101, 0b111, 0b101, 0b111 },
    // 9
    { 0b111, 0b101, 0b111, 0b001, 0b111 }
};


MatrixTime::MatrixTime(){
  _isRunning = false;
}


void MatrixTime::begin(){
  _isRunning = true;
  matrix.begin();
  _unixHrsOffs = 0;
  _unixMinOffs = 0;
}


void MatrixTime::start(){
  _isRunning = true;
  matrix.clear();
  matrix.play(true);
}


void MatrixTime::stop(){
  matrix.clear();
  matrix.play(false);
  _isRunning = false; 
}


void MatrixTime::clearMatrix() {
  _frame[0] = _frame[1]= _frame[2] = 0;
  matrix.loadFrame(_frame);
}


// RPC function called from web clock
void MatrixTime::updateTime() {

  if (_isRunning) {

    RTCTime ctime;
    RTC.getTime(ctime);
    long long int unixTime = ctime.getUnixTime() + _unixHrsOffs + _unixMinOffs;
    ctime = RTCTime(unixTime);

    buildClockBitmap((uint8_t)ctime.getHour(), (uint8_t)ctime.getMinutes(), (uint8_t)ctime.getSeconds());
    matrix.loadFrame(_frame);
  }
}


void MatrixTime::buildClockBitmap(uint8_t hour, uint8_t minute, uint8_t second) {

    uint8_t yOffset = 1; // hour digits anchored to lines 1 to 5

    // Reset all output bits
    _frame[0] = _frame[1] = _frame[2] = 0;

    // Get values for hour and minute 10s and units
    uint8_t hTens  = hour    / 10;
    uint8_t hUnits = hour    % 10;
    uint8_t mTens  = minute  / 10;
    uint8_t mUnits = minute  % 10;

    // Draw hours
   if (hour >= 10) drawDigit(hTens, 0, yOffset);
   drawDigit(hUnits, 3, yOffset);

    // Second and 10 second counters
    setPixelBit(6+(second/10),0);
    setPixelBit((second % 10),7);

    // Draw minutes - digits anchored one line lower
    drawDigit(mTens, 6, (yOffset+1)); 
    drawDigit(mUnits, 9, (yOffset+1));

}


void MatrixTime::drawDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset) {

    if (digit > 9) return;

    for (uint8_t row = 0; row < 5; row++) {
        uint8_t pattern = DIGITS[digit][row];

        for (uint8_t col = 0; col < 3; col++) {
            if (pattern & (1u << (2 - col))) {
                setPixelBit(xOffset + col, yOffset + row);
            }
        }
    }
}


void MatrixTime::setPixelBit(uint8_t x, uint8_t y) {
    if (x >= MATRIX_WIDTH) return;
    if (y >= MATRIX_HEIGHT) return;

    int index = (y * MATRIX_WIDTH) + x;  // 0 to 95
    int word  = index / 32;              // 0 to 2
    int bit   = index % 32;              // 0 to 31

    _frame[word] |= (1u << (31-bit));
}


uint8_t MatrixTime::displayHour(uint8_t hour){
  uint8_t dhour = hour;
  if (_hrmode == 12) {
    dhour = hour % 12;
    if (dhour == 0) dhour = 12;
  }
  return dhour;
}


void MatrixTime::setHourMode(uint8_t hrmode){
  if (hrmode == 12) {
    _hrmode = 12;
  }else{  
    _hrmode = 24;
  }
  matrix.clear();
  matrix.play(true);
}


// Time zone offset set from web clock 
void MatrixTime::setTimeOffset(char * offset){
  char toffs[10];
  int toHrs = 0;
  int toMin = 0;

Serial.print(F("Offset: "));
Serial.println(offset);

  memset(toffs, '\0', 10);
  strncpy(toffs, (offset + 1), 10);

Serial.print(F("Toffs: "));
Serial.println(toffs);

  toHrs = atoi(strtok(toffs, ":"));
  toMin = atoi(strtok(NULL, ":"));

  if (offset[0] == '-') {
    toHrs = -toHrs;
    toMin = -toMin;
  }

  _unixHrsOffs = toHrs * 3600;
  _unixMinOffs = toMin * 60;

Serial.print("OffsHrs: ");
Serial.println(toHrs);
Serial.print("OffsMin: ");
Serial.println(toMin);

Serial.print("OffsHrs: ");
Serial.println(_unixHrsOffs);
Serial.print("OffsMin: ");
Serial.println(_unixMinOffs);



}


long long int MatrixTime::getTimeDate(){
  RTCTime ctime;
  RTC.getTime(ctime);
  long long int unixtime = ctime.getUnixTime() + _unixHrsOffs + _unixMinOffs;
  return unixtime;
}
