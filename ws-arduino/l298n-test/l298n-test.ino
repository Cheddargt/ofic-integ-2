//Programa : Controle 2 motores DC usando Ponte H L298N
//Autor : FILIPEFLOP
 
//Definicoes pinos Arduino ligados a entrada da Ponte H
int IN1 = 12;
int IN2 = 13;
  
void setup()
{
  //Define os pinos como saida
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);

}
  
void loop()
{
 //Gira o Motor A no sentido horario -- pra tras
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 delay(4000);
 //Para o motor A
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, HIGH);
 delay(500);
 
 //Gira o Motor A no sentido anti-horario -- pra frente
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 delay(2000);
 //Para o motor A
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, HIGH);
 delay(500);

}
