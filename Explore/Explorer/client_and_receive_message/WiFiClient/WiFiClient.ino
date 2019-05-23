/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU


#ifndef STASSID
#define STASSID "jon snow est mort ep5 s8"
#define STAPSK  "azerty2a"
#endif


const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.43.170";
const uint16_t port = 8080;

boolean first_time = true;

String data; //Initialized variable to store recieved data


void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/
}
WiFiClient client;
void loop() {
  /*Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);*/

  // Use WiFiClient class to create TCP connections
  if (first_time && !client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  first_time = false;
  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    data = Serial.readString(); //Read the serial data and store it*
    Serial.println(data);
    if(data != "" || data != "\n") client.print(data);
    //client.println("hello from ESP8266");
  }

  // wait for data to be available
 /* unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }*/

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  /*Serial.println();
  Serial.println("closing connection");
  client.stop();*/

  //delay(300000); // execute once every 5 minutes, don't flood remote service
}
