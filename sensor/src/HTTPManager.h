#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <ESP8266WiFi.h>
#include <ProgConfig.h>
//#include <string.h>

//WiFiServer WIFIServer(80);

enum HTTPStatusCodes{
  HTTP_SUCCESS,
  HTTP_NULL_REQUEST,
  HTTP_MATCH_CREDENTIALS,
  HTTP_CREDENTIALS_MISSING,
  HTTP_INVALID,
  HTTP_INVALID_COMMAND,
  HTTP_INVALID_CREDENTIALS
};

class HTTPManager{
public:
  HTTPManager(int port);
  void begin();
  void update();
private:
  float pH;
  WiFiServer *server;
  void setupWiFiAP();
  String parseClientRequest(char *request,enum HTTPStatusCodes *err);
  void paramSet();
  void paramGet();
};

#endif
