#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <Arduino.h>

class SerialManager {
public:
  SerialManager(int baud);
  void Update();
};

#endif
