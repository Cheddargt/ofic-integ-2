#include <ezButton.h>

#define Fim_Curso_Inicio 4
#define Fim_Curso_Fim 9

#define PIR 8

#define Motor_IN1 12
#define Motor_IN2 13

ezButton fim_curso_inicio(Fim_Curso_Inicio);
ezButton fim_curso_fim(Fim_Curso_Fim);

int opcao_menu = 0; 
int presenca = 0;
int limpeza_completa = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  fim_curso_inicio.setDebounceTime(50); // set debounce time to 50 milliseconds
  fim_curso_fim.setDebounceTime(50);

  pinMode(PIR, INPUT);

  pinMode(Motor_IN1, OUTPUT);
  pinMode(Motor_IN2, OUTPUT);

  //digitalWrite(Motor_IN1, LOW);
  //digitalWrite(Motor_IN2, LOW);

  Serial.println("start");
}

void loop() {

  if (Serial.available() > 0)
{
  
  opcao_menu = Serial.read();

  //Solicita a limpeza
  if(opcao_menu == '1')
  {
    Serial.println("Limpeza solicitada.");
    
    
    
    //Verifica sensor de presença
    presenca = digitalRead(PIR);

    if(presenca == LOW) //Se não achou nada
    {
      Serial.println("Presenca nao detectada");
      
      delay(1000); //Espera 5000 ms
      presenca = digitalRead(PIR); //Verifica sensor de presença mais uma vez
      
      if(presenca == LOW)
      {
        Serial.println("Nova deteccao feita. Presenca nao detectada");

        Serial.println("Ligando motor.");
        
        fim_curso_inicio.loop(); //MUST
        fim_curso_fim.loop();
        
        while(!fim_curso_fim.isReleased())
        {
          digitalWrite(Motor_IN1, HIGH);
          digitalWrite(Motor_IN2, LOW);
          delay(1000);
        }
        presenca = digitalRead(PIR);
        if(presenca == HIGH)
        {
          Serial.println("Presenca detectada. Limpeza cancelada");
        }
        
        //Fim de curso ligado
        digitalWrite(Motor_IN1, HIGH); //PARA
        digitalWrite(Motor_IN2, HIGH);
        delay(100);

        Serial.println("Invertendo motor");

        //Inverte motor
        while(fim_curso_inicio.isReleased())
        {
          digitalWrite(Motor_IN1, LOW);
          digitalWrite(Motor_IN2, HIGH);
          delay(1000);
        }
        digitalWrite(Motor_IN1, HIGH); //PARA
        digitalWrite(Motor_IN2, HIGH);
        delay(100);

        presenca = digitalRead(PIR);
        if(presenca == HIGH)
        {
          Serial.println("Presenca detectada. Limpeza cancelada");
        }
  
        digitalWrite(Motor_IN1, LOW); //PONTO MORTO
        digitalWrite(Motor_IN2, LOW);
        delay(100);

        Serial.println("Motor desligado");
      }
      else
      {
        Serial.println("Detectou presenca. Limpeza cancelada \n");
      }
    }
    else
    {
      Serial.println("Presenca detectada. Espere e depois solicite de novo. \n");
    }

  }
}

}
