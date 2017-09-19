#include <SensorDataManager.h>

SensorDataManager::SensorDataManager(int sensorCount, uint32_t readInterval){
  sensorData.count = sensorCount;
  sensorData.item = new SensorHeader[sensorCount];
  sensorData.readInterval = readInterval;
}

void SensorDataManager::update(){
  if(_sm_readFlag){
    readSensors();
  }
  yield();
}

void SensorDataManager::setupTimer(){
    os_timer_setfn(&readTimer,_sm_timerCB,NULL);
    os_timer_arm(&readTimer,sensorData.readInterval,true);
}

void SensorDataManager::_sm_timerCB(void *pArg){
  _sm_readFlag = true;
}

void SensorDataManager::addSensor(char* name, float (*func)(), int sensorID){
  for(int i =0;i<sensorData.count;i++){
    if(!sensorData.item[i].is_this_used){
      sensorData.item[i].is_this_used = true;
      sensorData.item[i].data.name = name;
      sensorData.item[i].sensorID = sensorID;
      sensorData.item[i].func = func;
      yield();
    }
  }
}

void SensorDataManager::readSensors(){
  for(int i = 0;i<sensorData.count;i++){
    if(sensorData.item[i].is_this_used){
      sensorData.item[i].data.value = sensorData.item[i].func();
      yield();
    }
  }
  _sm_readFlag = false;
}

int SensorDataManager::findSensor(int sensorID){
  for(int i =0;i<sensorData.count;i++){
    if((sensorData.item[i].sensorID == sensorID) && sensorData.item[i].is_this_used){
      return i;
    }
  }
  return -1;
}

void SensorDataManager::setValue(int sensorID, float value){
  int count = findSensor(sensorID);
  if(count != -1){
    sensorData.item[count].data.value = value;
  }
}

void SensorDataManager::getValue(int sensorID,SensorValues *value){
  int count = findSensor(sensorID);
  if(count != -1){
    *value = sensorData.item[count].data;
  }
  value = NULL;
}
