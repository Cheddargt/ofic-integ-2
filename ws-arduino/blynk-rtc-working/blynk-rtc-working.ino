/********************** Blynk Setup **************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLc2ueuRLM"
#define BLYNK_DEVICE_NAME "Caixa de areia"
#define BLYNK_AUTH_TOKEN "gcsFsW4eDLtAoJPt82IzSCeyE1uFGXUe"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nelson";
char pass[] = "universo";
//char ssid[] = "BakerStreet221b";
//char pass[] = "ArseneLupin";
//char ssid[] = "zeni12";
//char pass[] = "paraquedas";


// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(6, 7); // RX, TX

// ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

// WidgetLED do Blynk
// WidgetLED led1(V7);
// WidgetLED led2(V13);

BlynkTimer timer;

WidgetRTC rtc;

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V2, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V17, currentDate);
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}


BLYNK_WRITE(V1) {
  Serial.print("V1 changed value: ");
  Serial.println(V1);
  if(param.asInt() == 1) {
    Serial.println("Limpeza solicitada");
    //Blynk.virtualWrite(V11, currentTime);
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

  // Other Time library functions can be used, like:
  //   timeStatus(), setSyncInterval(interval)...
  // Read more: http://www.pjrc.com/teensy/td_libs_Time.html

  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)

  // Display digital clock every 10 seconds
  timer.setInterval(10000L, clockDisplay);
}

void loop()
{
  Blynk.run();
  timer.run();
}
