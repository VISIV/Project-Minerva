
//---------------------------------------------------------
//               DEBUG
//---------------------------------------------------------
#define DEBUG true
#define ESP_DEBUG true
#define DEBUG_MODE UART // UART OR SD LOG
#if DEBUG
#if DEBUG_MODE == UART

#endif
#if DEBUG_MODE == SD

#endif
#endif
//---------------------------------------------------------
//               WIFI DEFINES
//---------------------------------------------------------
#define WIFI_SSID "ESP8266 "
#define WIFI_PSK "123456789"
#define WIFI_LOCAL_IP 192,168,1,1
#define WIFI_GATEWAY 192,168,1,1
#define WIFI_SUBNET 255,255,255,0

//---------------------------------------------------------
//               SERVER DEFINES
//---------------------------------------------------------
#define REQUEST_BUFFER_SIZE 255

//---------------------------------------------------------
//               TIMER DEFINES
//---------------------------------------------------------
#define RESPONSE_WAIT 5

//---------------------------------------------------------
//               EEPROM DEFINES
//---------------------------------------------------------
#define USERNAME_SIZE 32
#define PASSWORD_SIZE 32
#define SYSTEMID_SIZE 16

//---------------------------------------------------------
//               SENSOR DEFINES
//---------------------------------------------------------
#define MAX_SENSOR_NAME 30
#define HRSTOMS 360000
