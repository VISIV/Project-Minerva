#include <HTTPManager.h>

HTTPManager::HTTPManager(int port){
  server = new WiFiServer(80);
}

void HTTPManager::begin(){
  setupWiFiAP();
}

void HTTPManager::setupWiFiAP(){
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
  server->begin();
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

void HTTPManager::update(){
  HTTPStatusCodes err;
  WiFiClient client = server->available();
  if(!client){
    #if DEBUG
        //Serial.println("No client requests");
    #endif
    return;
  }

  // Read client request
  char requestBuffer[REQUEST_BUFFER_SIZE];
  memset(requestBuffer,0,REQUEST_BUFFER_SIZE);
  //String req = client.readStringUntil('\r');
  if(client.readBytesUntil('\r', requestBuffer, REQUEST_BUFFER_SIZE) > REQUEST_BUFFER_SIZE){
    Serial.println("Request Buffer overload");
  }

  #if DEBUG
      Serial.print("\n\nClient Request : ");
      Serial.println(requestBuffer);
  #endif
  client.flush();

  // Get client command here
  String content = sliceClientRequest(requestBuffer,&err);

  // Prepare response
  //String content = "Response";
  String header;
  if(err){
    header = "HTTP/1.1 200 OK\r\n";
  }else{
    header = "HTTP/1.1 400 Bad Request\r\n";
  }

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

String HTTPManager::sliceClientRequest(char *request,enum HTTPStatusCodes *err){
  String content = "";
  // Read Request type (GET,POST)
  // Not being used for now
  char *requestType = strtok(request,"/");
  #if DEBUG
    Serial.print("Request Type: ");
    Serial.println(requestType);
  #endif

  // Read command from android (paramSet,paramGet)
  char *reqCommand = strtok(NULL,"/");
  #if DEBUG
    Serial.printf("Command: -%s-\n",reqCommand);
  #endif
  // If command is paramSet
  if(!strcmp(reqCommand,"paramSet")){
    #if DEBUG
      Serial.println("Request Command: paramSet");
    #endif

    // Check Credentials from android app
    char *user = strtok(NULL,"/");
    char *pass = strtok(NULL,"/");
    #if DEBUG
      Serial.printf("Username: %s\nPassword: %s\n",user,pass);
    #endif

    // test credentials ----------------------------------------------- replace in release
    if( (!strcmp(user,"testuser")) && (!strcmp(pass,"testpass")) ){
      char *var = strtok(NULL,"/");
      if(!strcmp(var,"pH")){
        char *val = strtok(NULL,"/");
        Serial.print("Setting pH to: ");
        Serial.println(val);
      }
      else{
        *err = HTTP_INVALID;
        content += "Request Invalid:H_001";
        return content;
      }
    }
    // on credential mismatch, return status code
    else{
      *err = HTTP_INVALID_CREDENTIALS;
      content =+ "Request Invalid:H_002";
      return content;
    }
  }

  // If command is paramGet
  else if(!strcmp(reqCommand,"paramGet")){
    #if DEBUG
      Serial.println("Request Command: paramGet");
    #endif

  }
  else{
    *err = HTTP_INVALID_COMMAND;
    content =+ "Request Invalid:H_003";
    return content;
  }
}
#if DEBUG
#endif

void HTTPManager::paramSet(){
  char *reqCommand = strtok(NULL,"/");

}
void HTTPManager::paramGet(){

}



/*Request = Method Request-URL HTTP-1.1\r\n
Response = Header+message

Type: paramSet, paramGet
192.168.xxx.xxx/paramSet/~memcpy(stuct)/
GET /paramSet/user/pass/var/~hash/checksum/ HTTP-1.1\r\n
GET /paramGet/var/~hash/checksum/ HTTP-1.1\r\n

Header:
HTML\1.1 200 OK\r\n
Content-Length: content\r\n
Content-Type: data/parameters\r\n
Connection: Close\r\n\r\n


HTML\1.1 400 Bad Request
*/
