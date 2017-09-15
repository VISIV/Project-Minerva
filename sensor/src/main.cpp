#include <ESP8266WiFi.h>
#include <ProgConfig.h>
#include <SerialManager.h>
#include <UserData.h>
#include <HTTPManager.h>

// -------------------------------------------------------------------
//                           Class Object Declaration
// -------------------------------------------------------------------
SerialManager serialManager(9600);
HTTPManager httpManager(80);
UserData userData;

// -------------------------------------------------------------------

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
