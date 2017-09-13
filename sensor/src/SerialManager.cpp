#include <SerialManager.h>



SerialManager::SerialManager(int _baud){
  Serial.begin(_baud);
  #if DEBUG
    strcpy(username,"testuser");
    strcpy(password,"testpass");
  #endif


}

void SerialManager::Update(){
  if(!Serial.available()){
    return;
  }
  delay(50);
  Serial.println("Note: System stops whenever serial commands are called");
  Serial.println("Multi-threading is not supported to save stack size");
  //Serial.println("System exits serial command when there are no response in 10 secs");

  serialBuf = Serial.readStringUntil('\n');

  #if DEBUG
      Serial.print("Serial available: ");
      Serial.println(serialBuf);
  #endif

  if(!serialBuf.compareTo("eeprom")){
    EEPROM_Commands();
  }
  else{
    Serial.println("Invalid Serial Command. Returning to loop\n");
    return;
  }
  Serial.println("Returning to main loop");
}

void SerialManager::EEPROM_Commands(){
  Serial.println("EEPROM Commands: setUserConfig, readUserConfig");

  // Response imeout disabled for now
  // setupTimer();
  // serialStatus = WAITING_FOR_RESPONSE;
  // Serial.print("Waiting for Response.");
  // while(serialStatus == WAITING_FOR_RESPONSE){
  //   Serial.print(".");
  // }
  // if(serialStatus == NO_RESPONSE){
  //   Serial.println("Response Timeout");
  //   return;
  // }

  while(!Serial.available());
  serialBuf = Serial.readStringUntil('\n');
  Serial.flush();

  #if DEBUG
    Serial.print("Response: ");
    Serial.println(serialBuf);
  #endif

  if(!serialBuf.compareTo("setUserConfig")){
    EEPROM_setUserConfig();
  }else if(!serialBuf.compareTo("readUserConfig")){
    EEPROM_readUserConfig();
  }else{
    Serial.println("Invalid Serial Command. Returning to loop\n");
    return;
  }
}

void SerialManager::EEPROM_setUserConfig(){
  char _user[16],_pass[16];

  // Ask for System Username
  Serial.println("EEPROM setUserConfig: Input username");
  while(!Serial.available());
  serialBuf = Serial.readStringUntil('\n');
  strcpy(_user,serialBuf.c_str());
  Serial.flush();
  Serial.print("Username: ");
  Serial.println(_user);

  // Ask for System Password
  Serial.println("EEPROM setUserConfig: Input password");
  while(!Serial.available());
  serialBuf = Serial.readStringUntil('\n');
  strcpy(_pass,serialBuf.c_str());
  Serial.flush();
  Serial.print("Password: ");
  Serial.println(_pass);

  #if DEBUG
    Serial.print("Username:");
    Serial.println(username);
    Serial.print("Password:");
    Serial.println(password);
  #endif

  if( (!strcmp(_user,username)) && (!strcmp(_pass,password)) ){
    Serial.println("User Credentials match");
  }
  else{
    Serial.println("User Credentials mismatch");
  }

}

void SerialManager::EEPROM_readUserConfig(){

}

// Response imeout disabled for now
// void SerialManager::setupTimer(){
//   Serial.println("setupTimer Called");
//   noInterrupts();
//   timer0_isr_init();
//   timer0_attachInterrupt(noResponseCallback);
//   ticks = ESP.getCycleCount() + (RESPONSE_WAIT * cpuSpeed);
//   timer0_write(ticks);
//   interrupts();
// }
//
// void SerialManager::noResponseCallback(void){
//   serialStatus = NO_RESPONSE;
//   Serial.print("serialStatus = ");
//   Serial.println(serialStatus);
//   Serial.println("noResponseCallback Called");
//   noInterrupts();
// }
