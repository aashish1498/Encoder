/*
  Encoder.h - Library for managing rotary encoder actions.
  Created by Aashish Mehta, 13 September 2023.
  Released into the public domain.
*/
#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class Encoder
{
public:
    Encoder(int pinCLK, int pinDT, int pinSW);
    void begin();
    void updateButtonState();
    void updateRotaryPosition();
    void setButtonTimes(int doublePressTime, int longPressTime);
    int rotaryPosition;
    bool buttonTriggered;
    bool longPress;
    bool doublePress;
    bool clickAndHold;
    bool clockwiseDetected;
    bool antiClockwiseDetected;

private:
    void increasePosition();
    void decreasePosition();
    int _longPressTime = 700;
    int _doublePressTime = 300;
    int _maxPosition = 255;
    int _minPosition = 0;
    int _sensitivity = 3;
    int _pinClk;
    int _pinDt;
    int _pinSw;
    bool _buttonDown;
    long _buttonDownTime;
    bool _triggerPending;
    long _triggerPendingTime;
    int _clkState;
    int _prevClkState;
};

#endif
