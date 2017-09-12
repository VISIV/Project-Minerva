#include <SerialManager.h>

SerialManager::SerialManager(int baud){
  Serial.begin(baud);
}

void SerialManager::Update(){
  if(!Serial.available()){
    return;
  }
  delay(50);

  //char serialBuf[50];
  // memset(serialBuf,0,50);
  //readSerial(serialBuf);
  //Serial.readBytesUntil("\0",serialBuf,50);
  String serialBuf = Serial.readStringUntil('\0');
  //Serial.read

  #if DEBUG
      Serial.print("Serial available: ");
      Serial.println(serialBuf);
  #endif

  if(serialBuf.compareTo("eeprom")){
    Serial.println("EEPROM Commands: setUserConfig");

  }
  else{
    Serial.println("Invalid Serial Command. Returning to loop\n");
  }
}
