//Programa : Controle 2 motores DC usando Ponte H L298N
//Autor : FILIPEFLOP
 
//Definicoes pinos Arduino ligados a entrada da Ponte H
int IN1 = 12;
int IN2 = 13;

int sensor = 8;             
int state = LOW;             
int val = 0;    
  
void setup()
{
  //Define os pinos como saida
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(LED_BUILTIN, OUTPUT);      
 pinMode(sensor, INPUT);    
 Serial.begin(9600);   

}
  
void loop()
{
  val = digitalRead(sensor);   // lê sensor
  Serial.println(val);
  if (val == HIGH) {
    //Gira o Motor A no sentido horario -- pra perto do motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    delay(5000);
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       
    }
  }

  else {
    //Gira o Motor A no sentido horario -- pra perto do motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    
    if (state == HIGH){
      Serial.println("Motion stopped!");
      state = LOW;      
    }
  }
}
