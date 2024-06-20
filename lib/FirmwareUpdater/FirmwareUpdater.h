#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

BearSSL::PublicKey signPubKey{R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0I9njDWXAifmY/uwdKy8
wTCzbV+pdtU72ElL1z1VfsUQJqCI78/q8wc+EjWydiVPSO9tRN5AbUQqEwN+FHAL
5dqgkHv0qp2pN9dEDT9hlxzXHZsFwdLhD958OkDZqq7J2D5rNeuj/dwLCqtQvhnN
aTTiZZ0TZL5ydwv5kdN/rU6pNzzFEzNc0w42LGsoo/VWFdI/gmeWuqmZ7tML2FXd
qG0wWNPQWmhKZ9GoHkA28Lhkk/zYagzgbXX496YZVXU95zRt+VqmWpHjbb+G0zjZ
cfnvIt0xgGEWKvR/SyT/Cf6R1f9VAs6JI8HHiqCtiwFn54MrXlSzACZiOEjpxt1Y
2QIDAQAB
-----END PUBLIC KEY-----)"};
BearSSL::HashSHA256 hash;
BearSSL::SigningVerifier sign{&signPubKey};

void setupFirmwareUpdater() {
    httpUpdater.setup(&httpServer, "/");
    Update.installSignature(&hash, &sign);
    httpServer.begin();
}

void handleFirmwareUpdater() {
    httpServer.handleClient();
}
