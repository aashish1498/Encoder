#include "Encoder.h"

#include "Arduino.h"

Encoder::Encoder(int pinCLK, int pinDT, int pinSW) {
    _pinClk = pinCLK;
    _pinDt = pinDT;
    _pinSw = pinSW;
}

void Encoder::begin() {
    pinMode(_pinClk, INPUT);
    pinMode(_pinDt, INPUT);
    pinMode(_pinSw, INPUT);
    _prevClkState = digitalRead(_pinClk);
    digitalWrite(_pinSw, HIGH);
}

void Encoder::updateButtonState() {
    long currentTime = millis();
    if (!(digitalRead(_pinSw)) && !_buttonDown) {
        _buttonDown = true;
        _buttonDownTime = millis();
        Serial.println("Button down!");
    } else if (digitalRead(_pinSw) && _buttonDown) {
        _buttonDown = false;
        if (currentTime > _buttonDownTime + _longPressTime) {
            if (_triggerPending) {
                clickAndHold = true;
                _triggerPending = false;
            } else {
                longPress = true;
            }
        } else {
            if (_triggerPending && currentTime < _triggerPendingTime + _doublePressTime) {
                _triggerPending = false;
                doublePress = true;
            } else if (!_triggerPending) {
                _triggerPending = true;
                _triggerPendingTime = millis();
            }
        }
        Serial.println("Button up!");
    }
    if (!_buttonDown && _triggerPending && currentTime > _triggerPendingTime + _doublePressTime) {
        buttonTriggered = true;
        _triggerPending = false;
    }
}

void Encoder::updateRotaryPosition() {
    _clkState = digitalRead(_pinClk);
    if (_clkState != _prevClkState) {
        if (digitalRead(_pinDt) == _clkState) {
            increasePosition();
        } else {
            decreasePosition();
        }
        Serial.print("Position: ");
        Serial.println(rotaryPosition);
    }
    _prevClkState = _clkState;
}

void Encoder::setButtonTimes(int doublePressTime, int longPressTime) {
    _doublePressTime = doublePressTime;
    _longPressTime = longPressTime;
}

void Encoder::increasePosition() {
    rotaryPosition = min(rotaryPosition + _sensitivity, _maxPosition);
    clockwiseDetected = true;
}

void Encoder::decreasePosition() {
    rotaryPosition = max(rotaryPosition - _sensitivity, _minPosition);
    antiClockwiseDetected = true;
}