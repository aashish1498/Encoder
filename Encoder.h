/*
  Encoder.h - Library for managing rotary encoder actions.
  Created by Aashish Mehta, 13 September 2023.
  Released into the public domain.
*/
#ifndef Encoder_h
#define Encoder_h
#include "Arduino.h"

class Encoder {
   public:
    Encoder(int pinCLK, int pinDT, int pinSW);
    void begin();
    void updateButtonState();
    void updateRotaryPosition();
    void setButtonTimes(int doublePressTime, int longPressTime);
    void setDebug(bool debug);
    bool checkSinglePress();
    bool checkDoublePress();
    bool checkLongPress();
    bool checkPressAndHold();
    bool checkClockwise();
    bool checkAntiClockwise();
    int rotaryPosition;

   private:
    void handleClockwise();
    void handleAnticlockwise();
    void debugLog(String message);
    void handleButtonReleased(long currentTime);
    void handleLongPressRelease();
    bool encoderButtonDown();
    bool triggerTimeElapsed(long currentTime);
    int _longPressTime = 700;
    int _doublePressTime = 300;
    int _maxPosition = 255;
    int _minPosition = 0;
    int _sensitivity = 3;
    int _pinClk;
    int _pinDt;
    int _pinSw;
    int _clkState;
    int _prevClkState;
    bool _debug;
    bool _buttonDownRecorded;
    bool _triggerPending;
    bool _singlePress;
    bool _doublePress;
    bool _longPress;
    bool _pressAndHold;
    bool _clockwiseDetected;
    bool _antiClockwiseDetected;
    long _buttonDownTime;
    long _triggerPendingTime;
};

#endif
