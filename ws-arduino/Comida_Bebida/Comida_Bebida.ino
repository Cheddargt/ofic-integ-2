//Oficinas 2 - PetStation - 2022/1
//Membros: Gustavo, Ianca, Raissa
//Maio de 2022

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

//Variáveis para teste enquanto ainda não tem app
int tem_comida = 0; 
int ligar_agua = 0;
int desligar_agua = 0;
int opcao_menu = 0;

//Variáveis do código
int porcao = 5;  //quantidade de porções de comida tem que colocar
int nivel_agua = 0;
Servo dispenser;
Servo pote;

int readSensorAgua() {
  int water_level;
  digitalWrite(Agua_Power, HIGH);  // Liga sensor
  delay(10);              // delay
  water_level = analogRead(Agua_Pin);    // Lê valor
  digitalWrite(Agua_Power, LOW);   // Desliga
  return water_level;             
}


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); //para debugar
  Serial.println("start");
  
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


 
//  Serial.println("Menu: \n");
//  Serial.print("Selecione a opcao que deseja executar: \n");
//  Serial.print("1 - Solicitar comida \n");
//  Serial.print("2 - Ligar agua \n");
//  Serial.print("3 - Desligar agua \n");
//
//  delay(1000);

if (Serial.available() > 0)
{
  
  opcao_menu = Serial.read();

  Serial.println(opcao_menu);

  delay(1000);

  //Verifica se foi selecionado para colocar comida
  if(opcao_menu == '1')
  {
    //Serial.println("Comida solicitada");

    //Serial.println("Digite a quantidade de porcoes de comida que deseja:");
    
    //porcao = Serial.read();

    Serial.println((String)"Escolhido "+porcao+" porcoes de comida");
    
    //Limpa o pote de ração
    Serial.println("Iniciando limpeza do pote");

    pote.write(180);
    delay(1500);
    pote.write(0);

    Serial.println("Limpeza do pote finalizada");

    if(digitalRead(Sensor_Infra) == LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      Serial.print("Comida disponivel \r\n");
      tem_comida = true;
    }else{ //SENÃO, FAZ
      Serial.print("Nao tem comida \r\n");
      tem_comida = false;
    }
    
    //faz servo girar x vezes
    if(tem_comida)
    {
      Serial.println("Iniciando dispenser de comida \n");
      for(int i = 0; i < porcao; i++)
      {
        dispenser.detach();
        delay(1000);
        dispenser.attach(Servo_Dispenser);
        dispenser.write(180);
        delay(1000);
  
        Serial.println((String)"Colocada "+(i+1)+" porcao \n");
      }
      dispenser.write(0);
      dispenser.detach();
      Serial.println("Dispenser finalizado \n");
    }
      //se o infra-vermelho for low -> acabou comida
        //interrupação tá configurada no setup
  }
  //Verifica se foi selecionado para ligar água
  else if(opcao_menu == '2')
  {
    Serial.print("Ligar agua solicitado \n");
    //Verifica sensor de água
    nivel_agua = readSensorAgua();

    Serial.println((String)"Nivel da agua: "+nivel_agua+"\n");
      
      if(nivel_agua > 150)//Nível ok -> liga bomba
      {
        Serial.print("Nivel OK \n");

        digitalWrite(Relay_Pin, LOW);

        Serial.print("Bomba ligada \n");
      }
      else //Nível ruim -> sem água
      {
        Serial.print("Nivel de agua baixo. Nao vai ligar a bomba \n");
      }

      Serial.print("Agua ligada n");
  }
  
  //Verifica se foi selecionado para desligar água
  else if(opcao_menu == '3')
  {
    Serial.print("Desligar agua solicitado \n");

    digitalWrite(Relay_Pin, HIGH);
    
    //Verifica sensor de água
    nivel_agua = readSensorAgua();

    Serial.println((String)"Nivel da agua: "+nivel_agua+"\n");
      
      if(nivel_agua > 150)//Nível ok -> liga bomba
      {
        Serial.print("Nivel OK \n");
      }
      else //Nível ruim -> sem água
      {
        Serial.print("Nivel de agua baixo. Coloque mais agua \n");
      }

      Serial.print("Agua Desligada \n");
  }
 }
}
