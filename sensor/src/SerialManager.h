#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <Arduino.h>
#include <ProgConfig.h>
#include <string.h>
#include <Ticker.h>

enum SerialStatus{
  New,NO_RESPONSE,WAITING_FOR_RESPONSE,EEPROM_COMMANDS,EEPROM_USER,EEPROM_PASSWORD
};

static enum SerialStatus serialStatus;

class SerialManager {
public:

  SerialManager(int _baud);

  void Update();

private:
  String serialBuf;

  volatile unsigned long ticks;
  const unsigned long cpuSpeed = ESP.getCpuFreqMHz() * 100000;
  char username[16],password[16];

  //static void noResponseCallback(void);
  //void setupTimer();
  void EEPROM_Commands();
  void EEPROM_setUserConfig();
  void EEPROM_readUserConfig();
};
#endif
