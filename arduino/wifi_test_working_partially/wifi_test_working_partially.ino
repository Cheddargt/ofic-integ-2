/* https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-modulo-wifi-esp8266-esp-01 */

#include "WiFiEsp.h"
#include <SoftwareSerial.h>

SoftwareSerial esp8266(6,7);  /* pino 2 TX , pino 3 RX */
const char* ssid = "Nelson";//type your ssid
const char* password = "universo";//type your password
 
int ledPin = 13; // GPIO2 of ESP8266
WiFiEspServer server(80);

void setup(){
  Serial.begin(9600);         /* Opções para ajuste 9600, 19200, 57600 e */
  esp8266.begin(9600);        /* 115200 dBs */
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop(){
  
}
