#include <AlertApi.h>
#include <FirmwareUpdater.h>
#include <StateDisplay.h>
#include <WiFiManager.h>

// Shift register pins
const int latchPin = D3; // Pin connected to ST_CP of 74HC595
const int clockPin = D2; // Pin connected to SH_CP of 74HC595
const int dataPin = D4;  // Pin connected to DS of 74HC595

const int hardResetPin = D6; // Hard reset pin

const char* apName = "AirRaidAlertMap";

unsigned int currentState = 0;
unsigned int newState = 0;


boolean isHardResetPressed() {
  if (digitalRead(hardResetPin) == LOW) {
    // ensure it is indeed pressed
    delay(2000);
    if (digitalRead(hardResetPin) == LOW) {
      return true;
    }
  }
  return false;
}

void hardReset() {
  ESP.eraseConfig();
  delay(2000);
  ESP.reset();
  delay(2000);
}


void setup() {
    Serial.begin(115200);
    pinMode(hardResetPin, INPUT_PULLUP);
    initStateDisplay(latchPin, clockPin, dataPin);

    if (isHardResetPressed()) {
      Serial.println("Hard reset button pressed, resetting");
      hardReset();
    }

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    res = wm.autoConnect(apName); // anonymous ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    }
    else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
    }

    // setup OTA handler
    setupFirmwareUpdater();
}

void loop() {
  handleFirmwareUpdater();

  newState = updateAlertState(currentState);

  if (currentState != newState) {
    displayState(currentState, newState);
    currentState = newState;
  }
}
