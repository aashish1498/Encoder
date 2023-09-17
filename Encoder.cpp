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

void Encoder::setButtonTimes(int doublePressTime, int longPressTime) {
    _doublePressTime = doublePressTime;
    _longPressTime = longPressTime;
}

void Encoder::setDebug(bool debug) {
    _debug = debug;
}

bool Encoder::checkSinglePress() {
    bool result = _singlePress;
    _singlePress = false;
    return result;
}

bool Encoder::checkDoublePress() {
    bool result = _doublePress;
    _doublePress = false;
    return result;
}

bool Encoder::checkLongPress() {
    bool result = _longPress;
    _longPress = false;
    return result;
}

bool Encoder::checkPressAndHold() {
    bool result = _pressAndHold;
    _pressAndHold = false;
    return result;
}

bool Encoder::checkClockwise() {
    bool result = _clockwiseDetected;
    _clockwiseDetected = false;
    return result;
}

bool Encoder::checkAntiClockwise() {
    bool result = _antiClockwiseDetected;
    _antiClockwiseDetected = false;
    return result;
}

void Encoder::updateButtonState() {
    long currentTime = millis();
    if (_buttonDownRecorded) {
        if (!encoderButtonDown()) {
            handleButtonReleased(currentTime);
        }
    } else {
        if (triggerTimeElapsed(currentTime)) {
            _singlePress = true;
            _triggerPending = false;
        }
        if (encoderButtonDown()) {
            _buttonDownRecorded = true;
            _buttonDownTime = currentTime;
            debugLog("Button down!");
        }
    }
}

void Encoder::updateRotaryPosition() {
    _clkState = digitalRead(_pinClk);
    if (_clkState != _prevClkState) {
        if (digitalRead(_pinDt) == _clkState) {
            handleClockwise();
        } else {
            handleAnticlockwise();
        }
        debugLog("Position: " + String(rotaryPosition));
    }
    _prevClkState = _clkState;
}

void Encoder::handleButtonReleased(long currentTime) {
    _buttonDownRecorded = false;
    if (currentTime > _buttonDownTime + _longPressTime) {
        handleLongPressRelease();
    } else {
        if (_triggerPending && currentTime < _triggerPendingTime + _doublePressTime) {
            _triggerPending = false;
            _doublePress = true;
        } else if (!_triggerPending) {
            _triggerPending = true;
            _triggerPendingTime = currentTime;
        }
    }
    debugLog("Button up!");
}

void Encoder::handleLongPressRelease() {
    if (_triggerPending) {
        _pressAndHold = true;
        _triggerPending = false;
    } else {
        _longPress = true;
    }
}

void Encoder::handleClockwise() {
    rotaryPosition = min(rotaryPosition + _sensitivity, _maxPosition);
    _clockwiseDetected = true;
}

void Encoder::handleAnticlockwise() {
    rotaryPosition = max(rotaryPosition - _sensitivity, _minPosition);
    _antiClockwiseDetected = true;
}

void Encoder::debugLog(String message) {
    if (_debug) {
        Serial.println(message);
    }
}

bool Encoder::encoderButtonDown() {
    return !(digitalRead(_pinSw));
}

bool Encoder::triggerTimeElapsed(long currentTime) {
    return _triggerPending && currentTime > _triggerPendingTime + _doublePressTime;
}
