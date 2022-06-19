/*************************************************************
  AVISO!
    incluir bibliotecas:

  Blynk: Sketch > Include Library > Manage Libraries > Blynk
  BlynkESP8266-master.zip (../BlynkESP8266-master.zip.zip)
  WiFiEsp-master.zip (../WiFiEsp-master.zip.zip)
  Time-master.zip (../Time-master.zip)
 *************************************************************/

 
/********************** Blynk Setup **************************/

#define BLYNK_TEMPLATE_ID "TMPLvza6Qtqy"
#define BLYNK_DEVICE_NAME "Caixa de comida"
#define BLYNK_AUTH_TOKEN "PgzBeXW8_ZqGnAxHIC-S5H_z1N7jKAKJ"

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
//char ssid[] = "BakerStreet221b";
//char pass[] = "ArseneLupin";
char ssid[] = "zn";
char pass[] = "paraquedas";



// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(6, 7); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

WidgetRTC rtc;

/*************************************************************/
/***************  Setup - Caixa de comida ********************/

#include <Servo.h>

//Define os pinos
#define Sensor_Infra 2 //alerta
#define Servo_Dispenser 4
#define Servo_Pote 10
#define Agua_Power 11
#define Agua_Pin A1
#define Relay_Pin A2

//Define valores de angulo dos motores
#define Pos_Init_Dispenser 0
#define Pos_Init_Pote 0
#define Pos_Final_Pote 180

//Variáveis do código
int porcao = 2;  //quantidade de porções de comida tem que colocar
int nivel_agua = 0;
int ultimaComida = 12;
int intervaloComida = 0;
Servo dispenser;
Servo pote;
bool tem_comida = false; 
bool agua_solicitada = false;
bool agua_ligada = false;
bool comida_solicitada = false;

/*************************************************************/

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  //ultimaLimpeza = hour();

  //String currentTime = String(hour()) + ":" + minute() + ":" + second();
  //String currentDate = String(day()) + " " + month() + " " + year();

  // Send time to the App
  //Blynk.virtualWrite(V2, currentTime);
  // Send date to the App
  //Blynk.virtualWrite(V17, currentDate);
}

BLYNK_WRITE(V5) {
  //Serial.print("V5 changed value: ");
  //Serial.println(V5);
  if(param.asInt() == 1) {
    //Serial.println("Agua solicitada");
    agua_solicitada = true;
  } else {
    //Serial.println("Agua desligada");
    agua_solicitada = false;
  }
}
//
BLYNK_WRITE(V0) {
   int currentHour = hour();
    
  //Serial.print("V1 changed value: ");
  //Serial.println(V0);
  if(param.asInt() == 1) {
    //Serial.println("Comida solicitada");
    comida_solicitada = true;
  }

  // Send time to the App
  Blynk.virtualWrite(V12, currentHour);
}

BLYNK_WRITE(V3) {
  porcao = param.asInt();
  //Serial.print("V3 changed value: ");
  //Serial.println(V3);
  //Serial.print("Porções de comida: ");
  //Serial.println(porcao);
}

BLYNK_WRITE(V8) {
  intervaloComida = param.asInt();
  //Serial.print("V3 changed value: ");
  //Serial.println(V3);
  //Serial.print("Porções de comida: ");
  //Serial.println(porcao);
}

int readSensorAgua() {
  int water_level;
  digitalWrite(Agua_Power, HIGH);  // Liga sensor
  delay(10);              // delay
  water_level = analogRead(Agua_Pin);    // Lê valor
  digitalWrite(Agua_Power, LOW);   // Desliga
  return water_level;      
}

void ligar_desligar_agua() {
  if(agua_solicitada){
    //Serial.print("Ligar agua solicitado \n");
    //Verifica sensor de água
    nivel_agua = readSensorAgua();

    //Serial.println((String)"Nivel da agua: "+nivel_agua+"\n");
      if(nivel_agua > 150)  //Nível ok -> liga bomba 
      {
        //Serial.print("Nivel OK \n");
        digitalWrite(Relay_Pin, LOW);
        //Serial.print("Bomba ligada \n");
        // Envia 1 pra Água disponível
        Blynk.virtualWrite(V9, 1);
      }
      else //Nível ruim -> sem água
      {
        //Serial.print("Nivel de agua baixo. Nao vai ligar a bomba \n");
        // Envia 0 pra Água disponível
        Blynk.virtualWrite(V9, 0);
      }
        // Serial.print("Agua ligada n");
  }
  else
  {
    digitalWrite(Relay_Pin, HIGH);
  }
}
//
void solicita_comida(){
    int horaAtual = hour();
  
    if (horaAtual - (ultimaComida + intervaloComida)) {
      comida_solicitada = true;
    }
    //Serial.println("Comida solicitada");
    //Serial.println((String)"Escolhido "+porcao+" porcoes de comida");
    
    //Limpa o pote de ração
    //Serial.println("Iniciando limpeza do pote");
    if(comida_solicitada){
      ultimaComida = hour();
      pote.write(180);
      delay(1500);
      pote.write(0);
  
      //Serial.println("Limpeza do pote finalizada");
  
      if(digitalRead(Sensor_Infra) == LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
        //Serial.print("Comida disponivel \r\n");
        tem_comida = true;
        // Envia 1 pra Comida Disponível
        Blynk.virtualWrite(V1, 1);
        Blynk.virtualWrite(V12, horaAtual);
        
        
      }else{ //SENÃO, FAZ
        //Serial.print("Nao tem comida \r\n");
        tem_comida = false;
        // Envia 0 pra Comida Disponível
        Blynk.virtualWrite(V1, 0);
      }
      
      //faz servo girar x vezes
      if(tem_comida)
      {
        //Serial.println("Iniciando dispenser de comida \n");
        for(int i = 0; i < porcao; i++)
        {
          dispenser.detach();
          delay(1000);
          dispenser.attach(Servo_Dispenser);
          dispenser.write(180);
          delay(1000);
    
          //Serial.println((String)"Colocada "+(i+1)+" porcao \n");
        }
        dispenser.write(0);
        dispenser.detach();
        //Serial.println("Dispenser finalizado \n");
      }
      comida_solicitada = false;
    }
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

void setup() {
  Serial.begin(115200); //para debugar
  // put your setup code here, to run once:
    // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  pinMode(Agua_Power, OUTPUT);
  digitalWrite(Agua_Power, LOW);
  //Acho que da água ainda falta coisa pra inicializar

  digitalWrite(Relay_Pin, HIGH);
  pinMode(Relay_Pin, OUTPUT);
  
  timer.setInterval(10000L, readSensorAgua);  
  timer.setInterval(1000L, ligar_desligar_agua);
  timer.setInterval(1000L, solicita_comida);

  // configura intervalo de atualização do RTC
  setSyncInterval(10 * 60);
  // Display digital clock every 10 seconds
  //timer.setInterval(10000L, clockDisplay);
  
  dispenser.attach(Servo_Dispenser);
  pote.attach(Servo_Pote);

  pote.write(0);

  pinMode(Agua_Power, OUTPUT);
  digitalWrite(Agua_Power, LOW);
  //Acho que da água ainda falta coisa pra inicializar

  digitalWrite(Relay_Pin, HIGH);
  pinMode(Relay_Pin, OUTPUT);
  
  pinMode(Sensor_Infra, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}
