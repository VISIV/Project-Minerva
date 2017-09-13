#include <ESP8266WiFi.h>
#include <ProgConfig.h>
#include <SerialManager.h>
#include <UserData.h>

WiFiServer server(80);
int resp_index = 0;

// -------------------------------------------------------------------
//                           Class Object Declaration
// -------------------------------------------------------------------
SerialManager serialManager(9600);
UserData userData;
// -------------------------------------------------------------------
void setupWiFi(){
  WiFi.mode(WIFI_AP);
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  const char PSK[] = WIFI_PSK;

  WiFi.softAPmacAddress(mac);

  IPAddress local_ip(192,168,mac[WL_MAC_ADDR_LENGTH-2],mac[WL_MAC_ADDR_LENGTH-1]);
  IPAddress gateway(192,168,mac[WL_MAC_ADDR_LENGTH-2],mac[WL_MAC_ADDR_LENGTH-1]);
  IPAddress subnet(WIFI_SUBNET);


  WiFi.softAPConfig(local_ip, gateway, subnet);

  String macID =  String(mac[WL_MAC_ADDR_LENGTH -2],HEX) +
                  String(mac[WL_MAC_ADDR_LENGTH -1],HEX);
  macID.toUpperCase();

  String AP_SSID =  WIFI_SSID + macID;


  char SSID[AP_SSID.length() + 1];
  memset(SSID,0,AP_SSID.length() + 1);
  for(int i = 0; i<AP_SSID.length(); i++){
    SSID[i] = AP_SSID.charAt(i);
  }

  WiFi.softAP(SSID,PSK);
  server.begin();
  #if DEBUG
    Serial.println(String("AP Successfuly Started"));

    Serial.printf("\nSSID: %s", SSID);
    Serial.printf("\nPSK: %s", PSK);
    Serial.printf("\nIP Address: ");Serial.print(local_ip);
    Serial.printf("\nGateway: ");Serial.print(gateway);
    Serial.printf("\nSubnet Mask: ");Serial.print(subnet);
    Serial.printf("\nMAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);//Serial.print(mac,HEX);
  #endif
}

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

void serverUpdate(){
  uint8_t err;
  WiFiClient client = server.available();
  if(!client){
    #if DEBUG
        //Serial.println("No client requests");
    #endif
    return;
  }

  // Read client request
  String req = client.readStringUntil('\r');
  #if DEBUG
      Serial.println("Client Request : "+req);
  #endif
  client.flush();

  // Do requests here

  // Prepare response
  String content = prepareResponseContent(1,&err);
  String header = "HTTP/1.1 200 OK\r\n";
  header += "Content-Length: ";
  header += content.length();
  header += "\r\n";
  header += "Connection: close\r\n\r\n";

  client.print(header + content);
  delay(1);
  #if DEBUG
      Serial.println("Server Response : "+ header + content);
  #endif

  client.flush();

}

void setup(){
  #if DEBUG
    Serial.begin(9600);
    while(!Serial.available()){while(Serial.available()){Serial.read();}}
  #endif
  setupWiFi();

}

void loop(){
  #if DEBUG
    //Serial.println("Entering -- void loop() --");
  #endif
  serverUpdate();
}
