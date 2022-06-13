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
  ezButton (Library manager)
 *************************************************************/

//Programa : PET-STATION Sandbox
//Autores : Gustavo Zeni, Ianca Polizelo, Raissa Higa


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
//char ssid[] = "BakerStreet221b";
//char pass[] = "ArseneLupin";
char ssid[] = "Nelson";
char pass[] = "universo";
//char ssid[] = "zn";
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

/*************************************************************/
/****************  Setup - Caixa de areia ********************/

// fim de curso usando ezButton (tratamentos adicionais, etc)
#include <ezButton.h>

#define Fim_Curso_Inicio 4
#define Fim_Curso_Fim 9
 
//Definicoes pinos Arduino ligados a entrada da Ponte H
//int Motor_IN1 = 12;
//int Motor_IN2 = 13;
#define Motor_IN1 12
#define Motor_IN2 13

#define PIR 8

bool praFrente = true;
int presenca = 0;
bool limpeza_completa = true;
bool limpeza_solicitada = false;
int ultimaLimpeza = 12;
int intervaloLimpeza = 0;

ezButton fim_curso_inicio(Fim_Curso_Inicio);
ezButton fim_curso_fim(Fim_Curso_Fim);

//#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA

/*************************************************************/

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  ultimaLimpeza = hour();

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();

  // Send time to the App
  //Blynk.virtualWrite(V2, currentTime);
  // Send date to the App
  //Blynk.virtualWrite(V17, currentDate);
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

BLYNK_WRITE(V1) {
  int currentHour = hour();
  
  //Serial.print("V1 changed value: ");
  //Serial.println(V1);
  if(param.asInt() == 1) {
    //Serial.println("Limpeza solicitada");
    limpeza_solicitada = true;
    limpeza_completa = false;
  }

  // Send time to the App
  Blynk.virtualWrite(V2, currentHour);
}

// V13 LED Widget is on or off
/***
void checkLEDStatus()
{
  if (led2.getValue()) {
    //led2.on();
    //Serial.println("LED on V1: off");
    limpeza_completa = false;
    limpeza_solicitada = true;
  } else {
    //led2.off();
  }
}
***/

void realizarLimpeza() {
      int horaAtual = hour();

      if (horaAtual - (ultimaLimpeza + intervaloLimpeza)) {
        limpeza_solicitada = true;
        limpeza_completa = false;
      }
  
      //Verifica sensor de presença
      //presenca = digitalRead(PIR);
      if(limpeza_solicitada && !limpeza_completa){
        presenca = LOW;
        
        if(presenca == LOW) //Se não achou nada
        {
         
          //Serial.println("Presenca nao detectada");
          //Serial.println("Ligando motor.");
          
          fim_curso_inicio.loop(); //MUST
          fim_curso_fim.loop();
          
          if(fim_curso_fim.isPressed()) {
            //Serial.println("** Rastelo voltando **");
            praFrente = false;
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, HIGH);
            delay(500);          
            //digitalWrite(Motor_IN1, HIGH);
            //digitalWrite(Motor_IN2, LOW);
            //delay(5000);
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, LOW);
            delay(500);
          }
  
          if(fim_curso_fim.isReleased())
            praFrente = false;
  
          if(fim_curso_inicio.isPressed() && !praFrente) {
            //Serial.println("** Rastelo chegou! **");
            // ir um pouquinho pra frente pra sair do botão
            digitalWrite(Motor_IN1, LOW);
            digitalWrite(Motor_IN2, HIGH);
            delay(5000);
            praFrente = true;
            limpeza_completa = true;
            limpeza_solicitada = false;
            //led2.off();
            //Serial.println("** Limpeza completa! **");
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, HIGH);
          }
  
          if(fim_curso_inicio.isReleased()) {
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, HIGH);
          }
  
          int state_fim = fim_curso_fim.getState();
          int state_inicio = fim_curso_inicio.getState();
  
          if(state_fim == HIGH || state_inicio == HIGH) { // The limit switches: UNTOUCHED
            //Serial.println(praFrente);
            if (praFrente && !limpeza_completa) { 
              //Serial.println("** Indo em direção da rampa **");
              digitalWrite(Motor_IN1, LOW);
              digitalWrite(Motor_IN2, HIGH);
            } else if (!limpeza_completa) {
              //Serial.println("** Indo em direção do motor **");
              digitalWrite(Motor_IN1, HIGH);
              digitalWrite(Motor_IN2, LOW);
            }
            
          }
          else { // The limit switch: TOUCHED
            //Serial.println("** -- Invertendo sentido **");
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, HIGH);
            delay(500);
          }
        } else { // detectou presença
          //Serial.println("Presenca detectada. Limpeza interrompida");
          digitalWrite(Motor_IN1, HIGH);
          digitalWrite(Motor_IN2, HIGH);
          delay(100); //Mudar depois para um tempo maior
        }
      }
}
    
void setup()
{
  // Debug console
  Serial.begin(115200);

  // Blynk
  // Set ESP8266 baud rate 
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  // Blynk
  Blynk.begin(auth, wifi, ssid, pass);
  
  
  timer.setInterval(100L, realizarLimpeza);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  // configura intervalo de atualização do RTC
  setSyncInterval(10 * 60);
  // Display digital clock every 10 seconds
  //timer.setInterval(10000L, clockDisplay);

  // Blynk
  // timer.setInterval(1000L, blinkLedWidget);


  fim_curso_inicio.setDebounceTime(100); // set debounce time to 50 milliseconds
  fim_curso_fim.setDebounceTime(100);

  pinMode(PIR, INPUT);
  
  //Define os pinos como saida
  pinMode(Motor_IN1, OUTPUT);
  pinMode(Motor_IN2, OUTPUT);

  //Blynk.run();
}

void loop()
{
  /*
  if (limpeza_completa = true) {
    digitalWrite(Motor_IN1, HIGH);
    digitalWrite(Motor_IN2, HIGH);
  }
  */
  Blynk.run();
  timer.run();
  
}
