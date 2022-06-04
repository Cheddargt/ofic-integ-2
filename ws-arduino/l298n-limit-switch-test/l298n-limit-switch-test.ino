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
 
//Definicoes pinos Arduino ligados a entrada da Ponte H
int IN1 = 12;
int IN2 = 13;

bool praFrente = true;

ezButton limitSwitch(9);  // create ezButton object that attach to pin 7;
  
void setup()
{
  //Define os pinos como saida
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);

 Serial.begin(9600);
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  
}
  
void loop()
{

  limitSwitch.loop(); // MUST call the loop() function first

  if(limitSwitch.isPressed()) {
    Serial.println("** Invertendo sentido **");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    delay(500);
  }

  if(limitSwitch.isReleased())
    praFrente = !praFrente;

  int state = limitSwitch.getState();
  if(state == HIGH) { // The limit switch: UNTOUCHED
    Serial.println(praFrente);
    if (praFrente) { 
      Serial.println("** Indo em direção da rampa **");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } else {
      Serial.println("** Indo em direção do motor **");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    
  }
    else { // The limit switch: TOUCHED
//    Serial.println(praFrente);
//        if (praFrente) {
//      digitalWrite(IN1, LOW);
//      digitalWrite(IN2, HIGH);
//    } else {
//      digitalWrite(IN1, HIGH);
//      digitalWrite(IN2, LOW);
//    }
      Serial.println("** -- Invertendo sentido **");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      delay(500);
  }
    
}
