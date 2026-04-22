#ifndef WEBCLOCK_H
#define WEBCLOCK_H

#include <avr/pgmspace.h>
#include <WiFiS3.h>
#include <RTC.h>
#include "MatrixTime.h"
#include "secrets.h"


#define TIME_ZONE_LENGTH 64
#define TIME_OFFS_LENGTH 10

#define HEADER_TEXT_HTML 0
#define HEADER_APP_JSON 1
#define HEADER_TEXT_JAVASCRIPT 2
#define HEADER_TEXT_CSS 3

#define REQUEST_LINE_LENGTH 128
#define REQUEST_POST_SIZE 256
#define REQUEST_ELEM_COUNT 3

class binaryWebClock {

  public:

    binaryWebClock(WiFiServer& serverobj, MatrixTime& matrixClock);

    void start();
    bool clientDetected();
    void clientRespond();

  private:

    static const char binaryClockHTML[];
    static const char binaryClockCss[];
    static const char binaryClockJs[];

    WiFiServer& binClock;
    WiFiClient wclient;

    MatrixTime& matrixClk;

    void sendHttpHeader(uint8_t header);
//    void requestGet(char * uri);
    void processRequest(char * rqtype, char * uri);

    void processUri(char * uridata, char * elem[], char * keypairs);
    void findKeyValue(char * findkey, char * keypairs, char * keyval);
    void processGet(char * uridata);
    void processPost(char * uri, char * postdata);
    void uriToString (const char * uri, char * string);
    int getCharVal(const char c);

    void callApiGetFunc(char * func);
    void callApiPostFunc(char * func, char * keypairs);

    void apiGetTime();
    void apiGetHourMode();
    void apiGetTimeZone();

    void apiPostMatrixStart();
    void apiPostMatrixStop();
    void apiPostSetHourMode(char * keypairs);
    void apiPostSetTimeZone(char * keypairs);

    char _tzone[TIME_ZONE_LENGTH];
    char _toffs[TIME_OFFS_LENGTH];
    bool _isrunning;
    uint8_t _hrmode;
    int _tz = 0;

};


#endif    // WEBCLOCK_h