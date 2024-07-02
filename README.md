# iot-air-alert-map

ESP8266 firmware to display Air Raid Alerts in Ukraine on a physical map using LEDs.
It requires a running https://github.com/and3rson/raid server instance.

## Circuit schematics

![Circuit](./docs/air-raid-alert-map.drawio.svg)

## LEDs

LEDs numeric descriptions can be found [here](https://github.com/and3rson/raid/blob/8e7a753/raid/updater.go#L44-L68).

## Developemt
1. Create `./config/secrets.ini`, see `./config/secrets.ini.dist` for an example
2. Put SSL private key (used for binary signing) in `./ssl/private.key`
3. Install PlatformIO extension for VSCode
4. Open project in VSCode
