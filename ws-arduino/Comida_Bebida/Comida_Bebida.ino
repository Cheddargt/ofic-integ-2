//Oficinas 2 - PetStation - 2022/1
//Membros: Gustavo, Ianca, Raissa
//Maio de 2022

#include <Servo.h>

//Define os pinos
#define Sensor_Infra 0 //alerta
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
int pedir_comida = 0; 
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

void interrupcao()
{
  Serial.println("Interrupcao detectada");
  Serial.println("Acabou a comida");

  //i = 100; //Para ter certeza que saiu do loop do for. É pra se o break não funcionar
  //break; //para sair do for da comida, mas não sei se vai dar certo
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

  pinMode(Relay_Pin, OUTPUT);
  digitalWrite(Relay_Pin, HIGH);

  //Sobre a interrupção:
  //Sensor_Infra é a porta (0 quando é porta 2, e 1 quando é porta 3)
  //Função que executa quando a interrupção é detectada
  //Tipo de variação do sinal a interrupção é disparada
    //LOW: quando tensão = 0V
    //CHANGE: quando o pino muda de estado (0V pra 5V ou 5V pra 0V)
    //RISING: borda de subida, quando muda de 0V para 5V;
    //FALLING: borda de descida, quando muda de 5V para 0V
  //attachInterrupt(Sensor_Infra,interrupcao, RISING); 

}

void loop() {
  // put your main code here, to run repeatedly:


 
  //Serial.println("Menu:");
  //Serial.print("Selecione a opcao que deseja executar:");
  //Serial.print("1 - Solicitar comida");
  //Serial.print("2 - Ligar agua");
  //Serial.print("3 - Desligar agua");

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
//    for (int posicao = Pos_Init_Pote; posicao <= Pos_Final_Pote; posicao += 1) 
//    { 
//      pote.write(posicao);              
//      delay(15);                       
//    }
//    for (int posicao = Pos_Final_Pote; posicao >= Pos_Init_Pote; posicao -= 1) 
//    {
//      pote.write(posicao);              
//      delay(15);                       
//    }
    Serial.println("Limpeza do pote finalizada");
    
    //faz servo girar x vezes
    Serial.println("Iniciando dispenser de comida");
    for(int i = 0; i < porcao; i++)
    {
      dispenser.detach();
      delay(1000);
      dispenser.attach(Servo_Dispenser);
      dispenser.write(180);
      delay(1000);

      Serial.println((String)"Colocada "+(i+1)+" porcao");
    }
    dispenser.write(0);
    dispenser.detach();
    Serial.println("Dispenser finalizado");
      //se o infra-vermelho for low -> acabou comida
        //interrupação tá configurada no setup
  }
  //Verifica se foi selecionado para ligar água
  else if(opcao_menu == '2')
  {
    Serial.print("Ligar agua solicitado");
    //Verifica sensor de água
    nivel_agua = readSensorAgua();

    Serial.println((String)"Nivel da agua: "+nivel_agua);
      
      if(nivel_agua > 150)//Nível ok -> liga bomba
      {
        Serial.print("Nivel OK");

        digitalWrite(Relay_Pin, LOW);

        Serial.print("Bomba ligada");
      }
      else //Nível ruim -> sem água
      {
        Serial.print("Nivel de agua baixo. Nao vai ligar a bomba");
      }

      Serial.print("Agua ligada");
  }
  
  //Verifica se foi selecionado para desligar água
  else if(opcao_menu == '3')
  {
    Serial.print("Desligar agua solicitado");

    digitalWrite(Relay_Pin, HIGH);
    
    //Verifica sensor de água
    nivel_agua = readSensorAgua();

    Serial.println((String)"Nivel da agua: "+nivel_agua);
      
      if(nivel_agua > 370)//Nível ok -> liga bomba
      {
        Serial.print("Nivel OK");
      }
      else //Nível ruim -> sem água
      {
        Serial.print("Nivel de agua baixo. Coloque mais agua");
      }

      Serial.print("Agua Desligada");
  }
 }
}
