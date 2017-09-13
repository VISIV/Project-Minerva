#include <ESP8266WiFi.h>
#include <ProgConfig.h>
#include <SerialManager.h>
#include <UserData.h>
#include <HTTPManager.h>

//WiFiServer server(80);
int resp_index = 0;

// -------------------------------------------------------------------
//                           Class Object Declaration
// -------------------------------------------------------------------
SerialManager serialManager(9600);
HTTPManager httpManager(80);
UserData userData;

// -------------------------------------------------------------------
String prepareResponseContent(int type, uint8_t *e){
  String response;
  if(type == 1){
    //response = "response here";
    response = "<html>\r\n";
    response += "<body><h1>Hello, World! ";
    response += String(resp_index++);
    response += "</h1></body></html>\n";
  }
  else{
    response = "Invalid Request";
    *e = 1;
  }
  *e = 0;
  return response;
}

void setup(){
  #if DEBUG
    Serial.begin(9600);
    //while(!Serial.available()){while(Serial.available()){Serial.read();}}
  #endif
  httpManager.begin();
}

void loop(){
  #if DEBUG
    //Serial.println("Entering -- void loop() --");
  #endif
  httpManager.update();

}
