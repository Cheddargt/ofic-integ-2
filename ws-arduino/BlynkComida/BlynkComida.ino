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
char ssid[] = "BakerStreet221b";
char pass[] = "ArseneLupin";
//char ssid[] = "BakerStreet221b";
//char pass[] = "ArseneLupin";
//char ssid[] = "zn";
//char pass[] = "paraquedas";



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
int porcao = 1;  //quantidade de porções de comida tem que colocar
int nivel_agua = 0;
int ultimaComida = 30;
int intervaloComida = 29;
Servo dispenser;
Servo pote;
bool tem_comida = false; 
bool agua_solicitada = false;
bool agua_ligada = false;
bool comida_solicitada = false;

/*************************************************************/

BLYNK_WRITE(V5) {
  if(param.asInt() == 1) {
    agua_solicitada = true;
  } else {
    agua_solicitada = false;
  }
}

BLYNK_WRITE(V0) {
  if(param.asInt() == 1) {
    comida_solicitada = true;
  } else {
    comida_solicitada = false;
  }
}

BLYNK_WRITE(V3) {  
  porcao = param.asInt();
}

BLYNK_WRITE(V8) {
  intervaloComida = param.asInt();
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

      // Nível padrâo: 150
      if(nivel_agua > 300)  //Nível ok -> liga bomba 
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
        Blynk.virtualWrite(V5, 0);
        agua_solicitada = false;
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
    int horaAtual = second();

    // alternativa 1: salvar o horário agora e atualizar ao final do percurso
    // alternativa 2: enviar o horário pro blynk e pegar no final

    int agendamento = (horaAtual - (ultimaComida + intervaloComida));
    
    Serial.println(horaAtual);
    Serial.println(ultimaComida);
    Serial.println(intervaloComida);
    Serial.println(agendamento);
    if ((horaAtual - (ultimaComida + intervaloComida)) <= 0) {
      comida_solicitada = true;
    }
    
    //Serial.println("Comida solicitada");
    //Serial.println((String)"Escolhido "+porcao+" porcoes de comida");
    
    if(comida_solicitada){
      comida_solicitada = false;
      Blynk.virtualWrite(V0, 0);
      
      //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      if(digitalRead(Sensor_Infra) == LOW){ 
        tem_comida = true;
        // Envia 1 pra Comida Disponível
        Blynk.virtualWrite(V1, 1);
      }
      else{ 
        tem_comida = false;
        // Envia 0 pra Comida Disponível
        Blynk.virtualWrite(V1, 0);
      }
      
      if(tem_comida)
      {
        // Delay fix:
        time_t time_now_adjust = now();
        Blynk.virtualWrite(V12, horaAtual);
        ultimaComida = second();
        
        // Limpa o pote de ração
        // Problema com agendamento:
        pote.write(180);
        delay(1500);
        pote.write(0);
        
        // Faz o motor da dispenser girar i vezes
        for(int i = 0; i < porcao; i++)
        {
          dispenser.detach();
          
          delay(1000);
          dispenser.attach(Servo_Dispenser);
          dispenser.write(180);
          delay(1000);
        }
        dispenser.write(0);
        dispenser.detach();
        // Delay fix:
        setTime(time_now_adjust);
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
  // timer.setInterval(10000L, clockDisplay);
  
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
