#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <Event.h>
#include <Timer.h>

/*
  STATUS:
    -1 wifi configuration not set
    -2 wifi not connected after 3 attemps
    -3 connection to host failed
    -4 Unexpected response (http response status)
    -5 Invalid headers (http response)
    -6 Parsing json failed

     1 wifi connected
     2 host connected
     3 alert sent

  Passed to arduino:
    st:x              -> status
    me:x              -> meteo humidity value
    pg[...][checksum] -> program of a sprinkler or meteo and the checksum to verify the program is received correctly in arduino side
    rst               -> to tell arduino to reset the sprinklers to default values because his going to receive a new program

  Received from arduino:
    meteo -> get meteo info
    alert -> send alert of low water level
    ack   -> acknowledge that he has received an entire program for a sprinkler and that it can send the next one

  Received from smart watering server:
    pg[s,1,1;m,300;st,20;f,30;d,20]|pg[s,2,1;m,400;st,20;f,20;d,10]|pg[s,3,1;m,500;st,20;f,20;d,5]|pg[s,4,1;m,600;st,20;f,25;d,15]|pg[me,1] -> program of all the sprinklers and meteo
*/
 
#include "variable.h"


String splitSring(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for(int i = 0; i <= maxIndex && found <= index; i++) {
      if(data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



/*
 **********************************************************************
 **********************************************************************
 *  Methods used in Station mode, when connecting to an existing wifi
 **********************************************************************
 **********************************************************************
 */
bool connectToWifi() {
  if(!isWifiConfigSet) {
    Serial.write("st:-1");
    return false;
  }

  byte i = 0;
  
  // If is not connected to the wifi then try max 3 times
  for (i = 0; WiFi.status() != WL_CONNECTED && i < 3; i++) {   
      WiFi.begin(ssid, password);
      delay(5000);
  }       
  if(i == 3) {
    Serial.write("st:-2");
    return false;
  }
  //Serial.write("st:1");
  return true;
}


// Connect to HTTP server
bool connectToHost(const char* host, const int port) {  
  if (!client.connect(host, port)) {
      Serial.write("st:-3");
      return false;
  }    
  //Serial.write("st:2");
  return true;
}


// Send HTTP request
void sendRequest(const char* host, String uriStr) {
  client.print("GET ");
  client.print(uriStr);
  client.println(" HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println("Connection: close");    // close the connection with host after response has been received
  client.println();
}


bool isResponseFromServerOk() {
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (status == "HTTP/1.1 200 OK") {
    Serial.write("st:-4");
    return false;
  }  

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.write("st:-5");
    return false;
  }
  
  return true;  
}



/*
 ********************
 *  Meteo methods
 ********************
 */
String getPrecipitationData() {

  // Allocate JsonBuffer
  // TODO: SEE THIS SITE AND CHECK BUFFER SIZE!! 
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(9) + JSON_ARRAY_SIZE(3) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);  

  // Parse JSON object
  JsonObject& json = jsonBuffer.parseObject(client);
  
  if (!json.success()) {
    Serial.write("st:-6");
    return "-1";
  }

  return json["main"]["humidity"];
}



/*
 **************************************************
 *  Smart Watering Server communication methods
 **************************************************
 */
 void checkServer(void* p) {
  if(!connectToWifi()) {      
    return;
  }

  if(connectToHost(smartWateringHost, smartWateringHttpPort)) {         
    sendRequest(smartWateringHost,  String("/api/getprog?id=") + userId);
    
    if(isResponseFromServerOk()) {
      progStr = client.readString();
      progStr.trim();

      if(progStr != "") {
        progStr = "rst|" + progStr;
        sendProgramToArduino(true);  
      }      
    }
  }
}


void sendProgramToArduino(bool progWellReceived) {
  if(progWellReceived) {
    indexProg++;
  }
  String str = splitSring(progStr, '|', indexProg);
  str.trim(); // erase any posible whitespaces at both ends
  
  if(str != "") {  
    addChecksum(&str);
    Serial.write(str.c_str());
  } else {
    progStr = "";
    indexProg = -1;
  }
}


// calculates the total ascii value of the program string, just the part inside the brackets -> pg[...]
void addChecksum(String *str) {
  int total = 0;
  for(int i = 3; i < str->length() - 1; i++) {   // starts at 3 to leave out 'pg[' and finishes at length-2 to leave out ']' -> example: pg[s,1,1;m,300;st,20;f,30;d,20]
    total += str->charAt(i);
  }
  str->concat("[");
  str->concat(total);
  str->concat("]");
}


void stopTimer(int *id) {
  timer.stop(*id);
  *id = -1;
}



/*
 ****************************************************************************
 ****************************************************************************
 *  Methods used in Access Point mode, when creating its own wifi and server
 ****************************************************************************
 ****************************************************************************
 */
// When URI / is requested, send a web page with a form
void handleRoot() {  
  server.send(200, "text/html", "<form action=\"/config\" method=\"POST\">Wifi Name:<input type=\"text\" name=\"ssid\" placeholder=\"wifi name\"></br>Password:<input type=\"password\" name=\"pass\" placeholder=\"password\"></br>City:<input type=\"text\" name=\"city\" placeholder=\"city\"></br>City:<input type=\"text\" name=\"id\" placeholder=\"id\"></br></br></br><input type=\"submit\" value=\"Set Configuration\"></form>");
}


// when a POST request is made to URI /config
void handleConfig() {    
  if( !server.hasArg("id") || !server.hasArg("ssid") || !server.hasArg("pass") || !server.hasArg("city")
      || server.arg("id") == NULL || server.arg("ssid") == NULL || server.arg("pass") == NULL || server.arg("city") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  } 
  else {
    server.arg("id").toCharArray(userId, server.arg("id").length() + 1);
    server.arg("ssid").toCharArray(ssid, server.arg("ssid").length() + 1);
    server.arg("pass").toCharArray(password, server.arg("pass").length() + 1);
    server.arg("city").toCharArray(meteoCityID, server.arg("city").length() + 1);
    isWifiConfigSet = true;    

    server.send(200, "text/plain", "CONFIG_"+server.arg("id")+" "+server.arg("ssid")+" "+ server.arg("pass")+" "+server.arg("city")); 

    stopTimer(&checkServerTimerId);
    checkServerTimerId = timer.every(CHECK_SERVER_TIME, checkServer, NULL);
    checkServer(NULL);    
  }  
}


// Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}



/*
 *****************
 *****************
 *  SETUP & LOOP
 *****************
 *****************
 */
void setup() {
  delay(1000);
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA); // Acces point and station mode
  
  WiFi.softAP(myssid, mypassword);  // Set Access Point mode

  // Server configuration
  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/config", HTTP_POST, handleConfig); // Call the 'handleConfig' function when a POST request is made to URI "/config"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();

  // Disconnect from any previous wifi
  WiFi.disconnect();
  
  client.setTimeout(HTTP_TIMEOUT);  // set the maximum client waiting time  
}


void loop() {    
  timer.update(); // update the timers
  server.handleClient();  // listen for clients

  String str = "";

  if(Serial.available()) {
    str = Serial.readString();
    str.trim(); // erase any posible whitespaces at both ends
  }  

  if (str.startsWith("ack")) {
    sendProgramToArduino(splitSring(str, ':', 1).toInt());
  }
  
  if (!(str == "meteo" || str == "alert")) {
    return;
  }
   
  if(!connectToWifi()) {      
    return;
  }

  if(str == "alert") {
    if(connectToHost(smartWateringHost, smartWateringHttpPort)) {   
      sendRequest(smartWateringHost,  String("/api/alert?id=") + userId);

      if(isResponseFromServerOk()) {
        Serial.write("st:3");
      }
    }
  }

  if(str == "meteo") { 
    if(connectToHost(meteoHost, meteoHttpPort)) { 
      sendRequest(meteoHost, String("/data/2.5/weather?id=") + meteoCityID + "&appid=" + apiKey);
      
      if(isResponseFromServerOk()) {
        String prec = getPrecipitationData();
        
        if(prec != "-1") {   
          Serial.write((String("me:") + prec).c_str());
        }   
      }
    }
  }
}
