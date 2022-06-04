 /*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-limit-switch
 */

//Programa : Controle 2 motores DC usando Ponte H L298N
//Autor : FILIPEFLOP

#include <ezButton.h>

#define Fim_Curso_Inicio 4
#define Fim_Curso_Fim 9
 
//Definicoes pinos Arduino ligados a entrada da Ponte H
int Motor_IN1 = 12;
int Motor_IN2 = 13;

#define PIR 8

bool praFrente = true;
int opcao_menu = 0; 
int presenca = 0;
int limpeza_completa = true;

ezButton fim_curso_inicio(Fim_Curso_Inicio);
ezButton fim_curso_fim(Fim_Curso_Fim);
 
void setup()
{
  Serial.begin(9600); 
  
  fim_curso_inicio.setDebounceTime(50); // set debounce time to 50 milliseconds
  fim_curso_fim.setDebounceTime(50);

  pinMode(PIR, INPUT);
  
  //Define os pinos como saida
  pinMode(Motor_IN1, OUTPUT);
  pinMode(Motor_IN2, OUTPUT);

  Serial.println(" \n Start");
  Serial.println("Selecione a opção:");
  Serial.println("1 = limpeza");
}
  
void loop()
{

  if (Serial.available() > 0) {
    if (limpeza_completa == true) {
      Serial.println("\n \n \n Selecione a opção:");
      Serial.println("1 = limpeza");
      digitalWrite(Motor_IN1, HIGH);
      digitalWrite(Motor_IN2, HIGH);
      opcao_menu = Serial.read();
      if (opcao_menu == '1') {
        Serial.println("Limpeza solicitada.");
      }
    }
    //Solicita a limpeza
    if(opcao_menu == '1') {
      limpeza_completa = false;
      //Verifica sensor de presença
      presenca = digitalRead(PIR);

      if(presenca == LOW) //Se não achou nada
      {

        Serial.println("Presenca nao detectada");
        Serial.println("Ligando motor.");
        
        fim_curso_inicio.loop(); //MUST
        fim_curso_fim.loop();
        
        if(fim_curso_fim.isPressed()) {
          Serial.println("** Rastelo voltando **");
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

        if(fim_curso_inicio.isPressed()) {
          Serial.println("** Rastelo chegou! **");
          // ir um pouquinho pra frente pra sair do botão
          digitalWrite(Motor_IN1, LOW);
          digitalWrite(Motor_IN2, HIGH);
          delay(5000);
          praFrente = true;
          limpeza_completa = true;
          Serial.println("** Limpeza completa! **");
        }

        if(fim_curso_inicio.isReleased()) {
          digitalWrite(Motor_IN1, HIGH);
          digitalWrite(Motor_IN2, HIGH);
        }

        int state_fim = fim_curso_fim.getState();
        int state_inicio = fim_curso_inicio.getState();

        if(state_fim == HIGH || state_inicio == HIGH) { // The limit switches: UNTOUCHED
          Serial.println(praFrente);
          if (praFrente && !limpeza_completa) { 
            Serial.println("** Indo em direção da rampa **");
            digitalWrite(Motor_IN1, LOW);
            digitalWrite(Motor_IN2, HIGH);
          } else if (!limpeza_completa) {
            Serial.println("** Indo em direção do motor **");
            digitalWrite(Motor_IN1, HIGH);
            digitalWrite(Motor_IN2, LOW);
          }
          
        }
        else { // The limit switch: TOUCHED
          Serial.println("** -- Invertendo sentido **");
          digitalWrite(Motor_IN1, HIGH);
          digitalWrite(Motor_IN2, HIGH);
          delay(500);
        }
      } else { // detectou presença
        Serial.println("Presenca detectada. Limpeza interrompida");
        digitalWrite(Motor_IN1, HIGH);
        digitalWrite(Motor_IN2, HIGH);
        delay(100); //Mudar depois para um tempo maior
      }
    }
  }
}
