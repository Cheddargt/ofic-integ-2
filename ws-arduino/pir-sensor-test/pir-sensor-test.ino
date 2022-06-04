int sensor = 8;             
int state = LOW;             
int val = 0;                 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(sensor, INPUT);    
  Serial.begin(9600);       
}

void loop(){
  val = digitalRead(sensor);   // lê sensor
  Serial.println(val);
  if (val == HIGH) {           // se HIGH acende led
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(100);                
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       
    }
  } 
  else {
      digitalWrite(LED_BUILTIN, LOW); 
      delay(100);             //delay
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       
    }
  }
}
