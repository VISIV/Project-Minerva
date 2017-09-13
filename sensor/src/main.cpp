#include <ESP8266WiFi.h>
#include <ProgConfig.h>
#include <SerialManager.h>

WiFiServer server(80);
int resp_index = 0;

// -------------------------------------------------------------------
//                           Class Object Declaration
// -------------------------------------------------------------------
SerialManager serialManager(9600);
// -------------------------------------------------------------------
void setupWiFi(){
  WiFi.mode(WIFI_AP);
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  const char PSK[] = WIFI_PSK;
  // Store ESP MAC Address in variable
  WiFi.softAPmacAddress(mac);
  // Set last 2 bytes of mac address as IP and Gateway
  IPAddress local_ip(192,168,mac[WL_MAC_ADDR_LENGTH-2],mac[WL_MAC_ADDR_LENGTH-1]);
  IPAddress gateway(192,168,mac[WL_MAC_ADDR_LENGTH-2],mac[WL_MAC_ADDR_LENGTH-1]);
  IPAddress subnet(WIFI_SUBNET);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // Add last 2 bytes of MAc Address to AP SSID
  String macID =  String(mac[WL_MAC_ADDR_LENGTH -2],HEX) +
                  String(mac[WL_MAC_ADDR_LENGTH -1],HEX);
  macID.toUpperCase();
  String AP_SSID =  WIFI_SSID + macID;

  char SSID[AP_SSID.length() + 1];
  memset(SSID,0,AP_SSID.length() + 1);
  for(int i = 0; i<AP_SSID.length(); i++){
    SSID[i] = AP_SSID.charAt(i);
  }

  // Start AP and begin server
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

bool prepareResponseContent(String type, String response){
  #if DEBUG
      Serial.println("Client Requesting : " + type);
  #endif
  if(type.equals("paramGet")){
    //response = "response here";
    response = "<html>\r\n";
    response += "<body><h1>Hello, World! ";
    response += String(resp_index++);
    response += "</h1></body></html>\n";
  }
  else{
    // Set response to Invalid request
    response = "";
    return false;
  }
  return true;
}

void serverUpdate(){
  bool result = false;
  String requestType = "";
  String content = "";

  // Check if there is a request
  WiFiClient client = server.available();
  if(!client){
    // Return back if no requests
    return;
  }

  // Read client request
  String req = client.readStringUntil('\r');
  client.flush();

  requestType = "paramGet";

  #if DEBUG
      Serial.println("Client Request : " + req);
  #endif

  // Prepare response
  String header = "HTTP/1.1 ";
  // Request data based from request
  result = prepareResponseContent(requestType,content);
  if(result == true){
    // Set Response status code to 200 on valid request
    header += "200 OK\r\n";
  }
  else{
    // Set Response status code to 403 on bad request
    header += "403 Bad Request\r\n";
  }
  header += "Content-Length: ";
  header += content.length();
  header += "\r\n";
  header += "Connection: close\r\n\r\n";

  // Send response to client
  client.print(header + content);
  delay(1);
  client.flush();

  #if DEBUG
      Serial.println("Server Response : \n"+ header + content+"----------------");
  #endif
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
