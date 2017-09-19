#ifndef SENSORDATAMANAGER_H
#define SENSORDATAMANAGER_H
#include <ProgConfig.h>
#include <cstdint>
#include <Esp.h>

typedef struct _sensorvalues{
  char* name;
  float value = 0;
}SensorValues;

typedef struct _sensorItem{
  int sensorID = 0;
  SensorValues data;
  float (*func)();
  bool is_this_used = false;
}SensorHeader;

typedef struct _sensorData{
  int count;
  uint32_t readInterval;
  SensorHeader* item;
}SensorData;

class SensorDataManager{
public:
  SensorDataManager(int sensorCount, uint32_t readInterval);
  void setValue(int sensorID, float value);
  void getValue(int sensorID,SensorValues *value);
  void addSensor(char* name,  float (*func)(), int sensorID);
  void readSensors();
  void update();
private:
  SensorData sensorData;
  int findSensor(int sensorID);
  ETSTimer readTimer;
  static bool _sm_readFlag;
  static void _sm_timerCB(void *pArg);
  void setupTimer();
};

#endif
