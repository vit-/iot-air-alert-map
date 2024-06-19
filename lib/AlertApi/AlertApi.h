#include <Arduino.h>
#include <WiFiManager.h>

WiFiClient client;

unsigned int addStateBit(unsigned int state, int bitNum) {
  state = state | (1 << (bitNum - 1));
  return state;
}

unsigned int removeStateBit(unsigned int state, int bitNum) {
  state = state & ~(1 << (bitNum - 1));
  return state;
}

unsigned int updateStateBit(unsigned int state, int bitNum, bool bitState) {
  if (bitState) {
    return addStateBit(state, bitNum);
  }
  return removeStateBit(state, bitNum);
}

unsigned int processState(unsigned int state, String data) {
  int delimeterIndex = data.indexOf("=");
  int region = data.substring(2, delimeterIndex).toInt();
  bool regionState = data.substring(delimeterIndex + 1).toInt() > 0;

  return updateStateBit(state, region, regionState);
}

unsigned int processPacket(unsigned int state, String data) {
    Serial.print("Got packet: ");
    Serial.println(data);
    if (data == "a:wrong_api_key") {
      Serial.println("Please specify your API key!");
      delay(5000);
    }
    if (data.startsWith("s:")) {
      state = processState(state, data);
    }
    return state;
}

unsigned int handleData(unsigned int currentState, String data) {
    int newState = currentState;
    static String buffer;
    buffer += data;
    while (1) {
        int border = buffer.indexOf("\n");
        if (border == -1) {
            break;
        }
        newState = processPacket(newState, buffer.substring(0, border));
        buffer = buffer.substring(border + 1);
    }
    return newState;
}

void connectApi() {
    if (!client.connected()) {
        Serial.print("Connecting to Air Raid Alert API...");
        while (!client.connect(ALERT_API_HOST, ALERT_API_PORT))
        {
            Serial.println(" Failed.");
            delay(1000);
        }
        Serial.println(" Connected!");
        client.write(ALERT_API_KEY);
    }
}

unsigned int updateAlertState(unsigned int currentState) {
    connectApi();
    String data;
    while (client.available() > 0)
    {
        data += (char)client.read();
    }
    if (data.length()) {
        return handleData(currentState, data);
    }
    return currentState;
}
