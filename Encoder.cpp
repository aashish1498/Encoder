#include "Arduino.h"
#include "Encoder.h"

const int LONG_PRESS_TIME = 700;
const int DOUBLE_PRESS_TIME = 300;

int maxPosition = 255;
int minPosition = 0;

// bool _buttonDown = false;
// long _buttonDownTime = 0;

// bool _triggerPending = false;
// long _triggerPendingTime = 0;

bool buttonTriggered = false;
bool longPress = false;
bool doublePress = false;
bool clickAndHold = false;
bool clockwiseDetected = false;
bool antiClockwiseDetected = false;

// int rotaryPosition = 0;

int _clkState;
int _prevClkState;

Encoder::Encoder(int PinCLK, int PinDT, int PinSW)
{
    _pinClk = PinCLK;
    _pinDt = PinDT;
    _pinSw = PinSW;
}

void Encoder::begin() {
    pinMode(_pinClk, INPUT);
    pinMode(_pinDt, INPUT);
    pinMode(_pinSw, INPUT);
    _prevClkState = digitalRead(_pinClk);
    digitalWrite(_pinSw, HIGH); // Pull-Up resistor for switch
    
    maxPosition = 255;
}

void Encoder::updateButtonState()
{
    long currentTime = millis();
    if (!(digitalRead(_pinSw)) && !_buttonDown)
    {
        _buttonDown = true;
        _buttonDownTime = millis();
        Serial.println("Button down!");
    }
    else if (digitalRead(_pinSw) && _buttonDown)
    {
        _buttonDown = false;
        if (currentTime > _buttonDownTime + LONG_PRESS_TIME)
        {
            if (_triggerPending)
            {
                clickAndHold = true;
                _triggerPending = false;
            }
            else
            {
                longPress = true;
            }
        }
        else
        {
            if (_triggerPending && currentTime < _triggerPendingTime + DOUBLE_PRESS_TIME)
            {
                _triggerPending = false;
                doublePress = true;
            }
            else if (!_triggerPending)
            {
                _triggerPending = true;
                _triggerPendingTime = millis();
            }
        }
        Serial.println("Button up!");
    }
    if (!_buttonDown && _triggerPending && currentTime > _triggerPendingTime + DOUBLE_PRESS_TIME)
    {
        buttonTriggered = true;
        _triggerPending = false;
    }
    // delay(4);
}

void Encoder::updateRotaryPosition()
{
    _clkState = digitalRead(_pinClk);
    if (_clkState != _prevClkState)
    {
        // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise(?)
        if (digitalRead(_pinDt) == _clkState)
        {
            increasePosition();
        }
        else
        {
            decreasePosition();
        }
        Serial.print("Position: ");
        Serial.println(rotaryPosition);
    }
    _prevClkState = _clkState; // Updates the previous state of the outputA with the current state
                               // delay(40);
}

void Encoder::increasePosition()
{
    rotaryPosition = min(rotaryPosition + _sensitivity, maxPosition);
    clockwiseDetected = true;
}

void Encoder::decreasePosition()
{
    rotaryPosition = max(rotaryPosition - _sensitivity, minPosition);
    antiClockwiseDetected = true;
}