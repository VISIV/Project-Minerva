#include <HTTPManager.h>

HTTPManager::HTTPManager(int port){
  server = new WiFiServer(80);
}

void HTTPManager::begin(){
  setupWiFiAP();
  pH = 7.02;
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
  char *method = strtok(requestBuffer," ");
  char *request = strtok(NULL," ");
  char *httpversion = strtok(NULL," ");

  // Get client command here
  String content = parseClientRequest(request,&err);
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

String HTTPManager::parseClientRequest(char *request,enum HTTPStatusCodes *err){
  String content = "{\"start\":0,";
  // Read Request type (GET,POST)
  // Not being used for now
  if(request == NULL){
    *err = HTTP_NULL_REQUEST;
    content += "\"Error\":\"H_001\",\"end\":0}";
    return content;
  }
  int index = 1;
  char *command = strtok(request,"/");
  #if DEBUG
    Serial.print("Command: ");
    Serial.println(command);
  #endif
  bool user = false,pass = false,syID = false,login = false;


  while(command != NULL){
    bool commandDone = false;
    // Check whether ID, PS, and SID Credentials match
    if(!strncmp(command,"ID=",3)){
      if(!strcmp(command+3,"testuser")){
        #if DEBUG
          Serial.println("ID Match");
        #endif
        user = true;
      }
    }else if(!strncmp(command,"PS=",3)){
      if(!strcmp(command+3,"testpass")){
        #if DEBUG
          Serial.println("PS Match");
        #endif
        pass = true;
      }
    }else if(!strncmp(command,"SID=",4)){
      if(!strcmp(command+4,"testsystemID")){
        #if DEBUG
          Serial.println("SID Match");
        #endif
        syID = true;
      }
    }else if((user && pass && syID)){
      // ID, PS, SID needs to be first 3 entries in the request
      // otherwise, request will be invalid
      if(login == false){
        content += "\"login\":1,";
        login = true;
      }
      // Add commands here
      // just follow conditional format
      if(!strcmp(command,"getPH")){
        #if DEBUG
          Serial.println("getPH Command");
        #endif
        content += "\"pH\":" + String(pH)+ ",";
      }else if(!strncmp(command,"setPH=",6)){
        #if DEBUG
          Serial.println("setPH Command");
        #endif
        pH = atof(command + 6);
        content += "\"pH\":" + String(pH)+ ",";
      }
    }else if((index >= 3) && !(user && pass && syID)){
      #if DEBUG
        Serial.println("Credentials Mismatch");
      #endif
      *err = HTTP_CREDENTIALS_MISSING;
      content += "\"Error\":\"H_002\",\"end\":" + String(index) +"}";
      return content;
    }else{
      // If ID, PS, and SID are not found within first 3 entries or
      // if credentials mismatch, returns error
      #if DEBUG
        Serial.println("Credentials Missing");
      #endif
      *err = HTTP_CREDENTIALS_MISMATCH;
      content += "\"Error\":\"H_003\",\"end\":" + String(index) +"}";
      return content;
    }
    command = strtok(NULL,"/");
    index++;
  }

  content += "\"end\":" + String(index) +"}";
  return content;
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
