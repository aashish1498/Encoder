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
    Encoder(int PinCLK, int PinDT, int PinSW);
    void begin();
    void updateButtonState();
    void updateRotaryPosition();
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
    int _pinClk;
    int _pinDt;
    int _pinSw;
    bool _buttonDown;
    long _buttonDownTime;
    bool _triggerPending;
    long _triggerPendingTime;
    int _sensitivity = 3;
    int _clkState;
    int _prevClkState;
};

#endif
