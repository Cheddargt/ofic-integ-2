/*************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Blynk using a LED widget on your phone!

  App project setup:
    LED widget on V1
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLc2ueuRLM"
#define BLYNK_DEVICE_NAME "Caixa de areia"
#define BLYNK_AUTH_TOKEN "gcsFsW4eDLtAoJPt82IzSCeyE1uFGXUe"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nelson";
char pass[] = "universo";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(6, 7); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

WidgetLED led1(V7);
WidgetLED led2(V13);

BlynkTimer timer;

// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    Serial.println("LED on V1: off");
  } else {
    led1.on();
    Serial.println("LED on V1: on");
  }
}

// V1 LED Widget is blinking
void checkLEDStatus()
{
  if (led2.getValue()) {
    led2.off();
    Serial.println("LED on V1: off");
  } else {
    led2.on();
    Serial.println("LED on V1: on");
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  timer.setInterval(1000L, blinkLedWidget);
  checkLEDStatus();
}

void loop()
{
  Blynk.run();
  timer.run();
}
