#include <Arduino.h>

int _latchPin;  // pin connected to ST_CP of 74HC595
int _clockPin;  // pin connected to SH_CP of 74HC595
int _dataPin;   // pin connected to DS of 74HC595

int _stateBlinkCount;  // state change blink count
int _stateBlinkDelay;  // state change blink delay, in milliseconds


void writeRegister(unsigned int num) {
  Serial.print("Writing register: ");
  Serial.println(num);

  digitalWrite(_latchPin, LOW);

  uint8_t part;
  for (int i = 24; i >= 0; i = i - 8) {
    part = (num >> i) & 0xff;
    shiftOut(_dataPin, _clockPin, MSBFIRST, part);
  }

  digitalWrite(_latchPin, HIGH);
}

void displayState(unsigned int currentState, unsigned int newState) {
  for (int i = 0; i < _stateBlinkCount; i++) {
    writeRegister(newState);
    delay(_stateBlinkDelay);
    writeRegister(currentState);
    delay(_stateBlinkDelay);
  }
  writeRegister(newState);
}

void initStateDisplay(int latchPin, int clockPin, int dataPin, int stateBlinkCount = 3, int stateBlinkDelay = 200) {
    _latchPin = latchPin;
    _clockPin = clockPin;
    _dataPin = dataPin;
    _stateBlinkCount = stateBlinkCount;
    _stateBlinkDelay = stateBlinkDelay;

    //set pins to output so you can control the shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    // reset register bank
    displayState(-1, 0);
}
