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

#include <Adafruit_ESP8266.h>


#include <Timer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#define HTTP_TIMEOUT 5000 // max response time from server
#define CHECK_SERVER_TIME 300000  // every 5 minutes (expressed in milliseconds)


// Credentials when module is in access point mode - the wifi created by the module
const char *myssid = "Smart_Watering";
const char *mypassword = "smart12345";
ESP8266WebServer server(80);


// Credentials when module is in station mode - ssid and password of wifi to connect to
char* ssid = (char*)malloc(sizeof(char) * 100);
char* password = (char*)malloc(sizeof(char) * 100);
bool isWifiConfigSet = false;


// Meteo site
const int meteoHttpPort = 80;
const char* meteoHost = "api.openweathermap.org";
const char* apiKey = "217b07a5c3c0dc0c6036378abf0a750f";

// City to search the forecast of
char* meteoCityID = (char*)malloc(sizeof(char) * 20);


// Smart watering server
const int smartWateringHttpPort = 8080;
const char* smartWateringHost = "134.59.129.150";


// String with sprinklers program and user ID
WiFiClient client;
String progStr;
byte indexProg = -1;
char* userId = (char*)malloc(sizeof(char) * 100);

Timer timer;
int checkServerTimerId = -1;
