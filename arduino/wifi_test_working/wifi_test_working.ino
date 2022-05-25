/*  */

#include <SoftwareSerial.h>

SoftwareSerial esp8266(6,7);  /* pino 6 TX , pino 7 RX */
void setup(){
Serial.begin(9600);         /* Opções para ajuste 9600, 19200, 57600 e */
esp8266.begin(9600);        /* 115200 dBs */
}
void loop(){
if(esp8266.available())       /* Confere se a comunicação está acessível */
{
while(esp8266.available()) {  /* Enquanto estiver acessível */
char c = esp8266.read();      /* Le o caractere. */
Serial.write(c);              /* Escreve no monitor serial */
}
}
if(Serial.available()){ 
delay(1000); 
String command="";
while(Serial.available()) {   /* Le o comando */
command+=(char)Serial.read();
}
esp8266.println(command);    /* Envia o comando para o ESP */
}
}
