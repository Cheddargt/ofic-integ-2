/*************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Blynk using a LED widget on your phone!

  App project setup:
    LED widget on V1

/*************************************************************
  AVISO!
    incluir bibliotecas:
    
  BlynkESP8266-master.zip
  WiFiEsp-master.zip
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
//char ssid[] = "Nelson";
//char pass[] = "universo";
char ssid[] = "BakerStreet221b";
char pass[] = "ArseneLupin";

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(6, 7); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

bool solicitacao_recebida = false;

BLYNK_WRITE(V1) {
  Serial.print("V1 changed value: ");
  Serial.println(V1);
  if(param.asInt() == 1) {
    Serial.println("Solicitacao recebida");
    solicitacao_recebida = true;
  }
}

void contarAteDez() {
  int i = 0;
  for (i = 0; i < 1000; i++) {
    Serial.print("Contando: ");
    Serial.println(i);
  }
  solicitacao_recebida = false;
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

}

void loop()
{
  if (!solicitacao_recebida) {
    Blynk.run();
  } else {
    contarAteDez();
  }
}
