#include <AlertApi.h>
#include <StateDisplay.h>
#include <WiFiManager.h>


//Pin connected to ST_CP of 74HC595
const int latchPin = D3;
//Pin connected to SH_CP of 74HC595
const int clockPin = D2;
//Pin connected to DS of 74HC595
const int dataPin = D4;

unsigned int currentState = 0;
unsigned int newState = 0;


void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    Serial.begin(115200);
    initStateDisplay(latchPin, clockPin, dataPin);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("AirRaidAlertMap"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

}

void loop() {
  newState = updateAlertState(currentState);

  if (currentState != newState) {
    displayState(currentState, newState);
    currentState = newState;
  }
}
