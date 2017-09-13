#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <ESP8266WiFi.h>
#include <ProgConfig.h>

//WiFiServer WIFIServer(80);

class HTTPManager{
public:
  HTTPManager(int port);
  void begin();
  void update();
private:
  WiFiServer *server;
  void setupWiFiAP();
};

#endif
