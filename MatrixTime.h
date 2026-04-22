#ifndef MATRIXTIME_H
#define MATRIXTIME_H

#include <RTC.h>
#include "Arduino_LED_Matrix.h"


class MatrixTime {

  public:
  
    MatrixTime();

    void begin();
    void start();
    void stop();
//    void updateTime(int32_t hour, int32_t minute, int32_t second);
    void updateTime();
    void clearMatrix();
    uint8_t displayHour(uint8_t hour);
    void setHourMode(uint8_t hrmode);
    void setTimeOffset(char * offset);
    long long int getTimeDate();
  
  
  private:

    ArduinoLEDMatrix matrix;

    void buildClockBitmap(uint8_t hour, uint8_t minute, uint8_t second);
    void drawDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset);
    void setPixelBit(uint8_t x, uint8_t y);
    uint32_t _frame[3];

    bool _isRunning;
    uint8_t _hrmode;
//    long int _unixHrsOffs;
//    long int _unixMinOffs;
    int32_t _unixHrsOffs;
    int32_t _unixMinOffs;

};


#endif	// MATRIXTIME_H

