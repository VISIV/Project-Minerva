#ifndef USERDATA_H
#define USERDATA_H

#include <EEPROM.h>

struct UserConfig{
  char userName[32];
  char password[32];
  char systemID[16];
};

class UserData{
public:
  void readUserConfig();
  void getUsername(char *user);
  void getPassword(char *pass);
  void getSystemID(char *sysID);
};

#endif
