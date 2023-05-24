#include <SoftwareSerial.h>
#include <Stepper.h>

#define e1 8
#define e2 9
#define e3 10
#define e4 11
#define rxPin 12
#define txPin 13


#define min_lambda 300
#define max_lambda 800


const int passosPorGiro = 64; //Quantidade de passos por giro neste modelo de motor, nesse caso, ele girará 1/passosPorGiro
Stepper mp(passosPorGiro, e1, e3, e2, e4); //Definindo o motor de passos; e3 e e2 tem que estar invertidos


SoftwareSerial mySerial = SoftwareSerial(rxPin,txPin);
int analogPin = A2;
int i;
int k;

int stepCounter = 0;
int aux = 0;

int Npontos = 0;
int Nmedias = 0;
int Npassos = 0;

unsigned int val;
int start_lambda, end_lambda;

int intervalo;

void resetStepper(Stepper *stepper, int nSteps, int passosPorGiroTotal){
  /*FUNÇÃO QUE RETORNA O MOTOR À SUA POSIÇÃO INICIAL, COM UMA COMPENSAÇÃO POR UMA POSSÍVEL FOLGA MECÂNICA, DEFINIDA PELO OFFSET*/
  int offset = 3;
  
  int i = 0;
  for(i = 0; i < nSteps + offset; i++){
    (*stepper).step(-passosPorGiroTotal);
  }

  for(i = 0; i < offset; i++){
    (*stepper).step(passosPorGiroTotal);  
  }
  
}




void stepAtTime(Stepper *stepper, int *n_cycles, int *stepCounter , int intervalo, int passosPorGiroTotal){
  /*FUNÇÃO QUE ROTACIONA O MOTOR A CADA N CICLOS*/
  if(*n_cycles == intervalo){
    (*stepper).step(passosPorGiro); 
    *n_cycles = 0; //atualiza a variável n_cycles 
    (*stepCounter)++; //Aumenta o número de passos
  }
  
}




void setup() {

  mp.setSpeed(300); //Velocidade de operação, em RPM. Aparentemente 300 é uma velocidade boa, há consistência nas giradas
  
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  mySerial.print(Npontos);
    
              
}


void loop() {



  
  if(Serial.available()){
    
    switch(Serial.read()){
      case 'a':


        //Coletando os parâmetros
        Serial.println("Quantos pontos deseja coletar?");
        while(Npontos<1){
          Npontos = Serial.parseInt();
          mySerial.println(Npontos);
        }
        Serial.println("Digite o número para as médias:");
        while(Nmedias<1){
          Nmedias = Serial.parseInt();
          mySerial.println(Nmedias);
        }
        Serial.println("Quantos passos deseja dar?");
        while(Npassos<1){
          Npassos = Serial.parseInt();
          mySerial.println(Npassos);
        }
        

       

        intervalo = int(Npontos/Npassos);

        stepCounter = 0;
      
        mySerial.println('a'); //Comunica ao outro arduíno o começo da medida dos pontos


        //Coleta de pontos
        for (i=0;i < Npontos; i++){
          
          val = 0;

          //Tira a média do ponto medido          
          for (k=0;k<=Nmedias;k++){
            val = val+ map(analogRead(analogPin), 0, 1023, 25, 180);
          }

          aux++;


          //Girando o motor no intervalo de tempo certo
          stepAtTime(&mp, &aux, &stepCounter , intervalo, passosPorGiro);
          delay(25);

          //Enviando o valor das medidas para o display
          Serial.println(val/Nmedias);
          mySerial.println((val/Nmedias));
        
       }

        //reseta o motor para a posição inicial
        resetStepper(&mp, stepCounter, passosPorGiro);
      }

       Npontos=0;
       Nmedias=0;
       Npassos=0;
    }
  }




  
