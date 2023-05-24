//Bibliotecas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>            // BIBLIOTECA LDC SERIAL, RESERVA PORTAs A4 SCA e A5 SCL PARA COMUNICAÇÃO COM LCD 
LiquidCrystal_I2C lcd(0x27, 16, 2);           // INFORMA PARA BIBLIOTECA QUE O END DO DISPLAY LCD É 0x27


//DEFINIÇÕES DAS PORTAS

#define Nmedidas 30
  
  int   ADC0 = A0;                            // Pino A0 - Entrada Analógica para Identificar os Sensores
  int   ADC1 = A1;                            // PINO A1 - Entrada ANALÓGICA do CONECTOR 1
  int   ADC2 = A2;                            // PINO A2 - Entrada ANALÓGICA do CONECTOR 2
  int   ADC3 = A3;                            // PINO A3 - Entrada ANALOGICA do CONECTOR 3
  //    A4 e A5                               // usadas para para comunicação Serial com o LCD 
 
  int   S1 = 2, S2 = 3, S3 = 4, S4 = 5;       // PORTA D2, D3, D4, D5; ENTRADA DIGITAL SENSORES 1,2,3 e 4 RESPECTIVAMENTE 
 
  byte  A = 6, B = 7;                         // Pinos D6 = A e D7 = B Multiplex 4052 para A0, Leitura Portas identificadoras tipo do Sensor
  
  int   BotaoAM = 8;                          // Chave Botão AMARELO
  int   BotaoVD = 9;                          // Chave Botão VERDE
  int   BotaoVM = 10;                         // Chave Botão VERMELHO
  byte  LedAM   = 11;                         // LED da Chave Botão AMARELO
  byte  LedVD   = 12;                         // LED da Chave Botão VERDE
  byte  LedVM   = 13;                         // LED da Chave Botão VERMELHO
  
  int redstate = HIGH ;
  int greenstate = HIGH ;
  int lrstate = HIGH ;
  
// DEFINIÇÕES DAS VARIÁVEIS

  int a = 1;                                  // Armazena a Função que será executada
  int b = 0;                                  // Tipo do Sensor Instalado ==> b=0 Nenhum Sensor Instalado
  int c = 0;                                  // Quantos Sensores Cronômetros estão Instalados        
  int i = 0;                                  // Usado para Loops For

  unsigned int t0 = 0;
  unsigned int t1 = 0;
  unsigned int T = 0;
  int nav=0;
  int AD = 0;
  int transicao = 0;
  int tr0 = 0;
  int medidas[30];

  byte customChar0[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
  
  int countSum = 0;                           // Variável para contador

  int geral = 0;                              // Variável de uso geral

  float Tempo_inicial, Tempo_final, Periodo[20];  // Variáveis Para a Rotina Pendulo()
  float Periodo_medio;                            // Variáveis Para a Rotina Pendulo()
  
  int sensor[5];                              // Variável para armazenar os Valores dos ADCs dos Sensores, Tipo do Sensor Conectado 
  
  char *funcao[6] = {"", "   CONTADOR     ", "    PENDULO     ", " TEMPO PASSAGEM ", "   CRONOMETRO   ", " FREQUENCIMETRO "}; 


//Variaveis para o Frequencimetro
// --- Variáveis Globais ---
long int counter;
int pulseCount;
boolean pulse;
const float Pi = 3.14159;
float freq;
#define inFreq   2  //entrada para medir a frequência no pino digital 2
unsigned long duracaoHigh, duracaoLow;                     //variável para guardar a duração do pulso Frequencimetro

 //Variaveis para o Cronometro Simples

double j = 0;
double k = millis();
double p ;
int x=0;
   
//CONFIGURANDO O HARDWARE
   
void setup() 
{
    Serial.begin(9600);
    lcd.createChar(0, customChar0);

    pinMode (A, OUTPUT);                          // Define Porta 6 como saída - Pino A seletor Multiplex 4052
    pinMode (B, OUTPUT);                          // Define Porta 7 como saída - Pino B seletor Multiplex 4052

    pinMode (S1, INPUT);                          // Define PINOS S1, S2, S3, S4 como ENTRADA DIGITAL, SENSORES 1,2,3 e 4 RESPECTIVAMENTE
    pinMode (S2, INPUT);
    pinMode (S3, INPUT);
    pinMode (S4, INPUT);
    
    pinMode (BotaoAM, INPUT_PULLUP);              // Define Pino Chave BotaoAM como ENTRADA 
    pinMode (BotaoVD, INPUT_PULLUP);              // Define Pino Chave BotaoVD como ENTRADA
    pinMode (BotaoVM, INPUT_PULLUP);              // Define Pino Chave BotaoVM como ENTRADA
    
    pinMode (LedAM, OUTPUT);                      // Define Pino LedAM da Chave AMARELA como SAÍDA
    pinMode (LedVD, OUTPUT);                      // Define Pino LedVD da Chave VERDE como SAÍDA
    pinMode (LedVM, OUTPUT);                      // Define Pino LedVM da Chave VERMELHA como SAÍDA

    // As Entradas Analogicas A0, A1, A2, A4 são inicializadas quando chamadas pela função analogRead()
    // As Saídas A4 e A5 são inicializadas pela biblioteca do LCD Serial <LiquidCrystal_PCF8574.h>


    Serial.begin(9600);                          // Inicializa comunicação Serial

//-----------------------------------------------------------------------------------------------------
    
    // Mensagem de Abertura

    lcd.init();                           // INICIALIZA LCD
    
    lcd.setBacklight(255);                      // LIGA BACKLIGHT
    
    digitalWrite(LedAM,HIGH);                   // Liga Led Chave AMARELA
    digitalWrite(LedVD,HIGH);                   // Liga Led Chave VERDE
    digitalWrite(LedVM,HIGH);                   // Liga Led Chave VERMELHA
    
    lcd.home(); lcd.clear();                    // RETORNA CURSOR DO LCD PARA O CANTO SUPERIOR ESQUERDO (0,0) E LIMPA O DISPLAY
    delay(1000);                                // ESPERA 1S
    lcd.print("USP - Sao Carlos");                // IMPRIME MENSAGEM DE ABERTURA NA LINHA 0
    lcd.setCursor(0,1);                         // MUDA CURSOR PARA COLUNA 0 E LINHA 2 (0,1)
    lcd.print("    IFSC-USP");                  // IMPRIME MENSAGEM DE ABERTURA NA LINHA 2
    
    delay(2000);                                // ESPERA 2 SEGUNDOS

    for(i=0;i<3;i++)
      {                // Pisca Back Light LCD e LEDs das Chaves 5 VEZES
       lcd.setBacklight(0);                     // DESLIGA BACKLIGHT
       digitalWrite(LedAM, LOW);                // DESLIGA Led Chave AMARELA
       digitalWrite(LedVD, LOW);                // DESLIGA Led Chave VERDE
       digitalWrite(LedVM, LOW);                // DESLIGA Led Chave VERMELHA
       delay(150);                              // ESPERA 150ms
 
       lcd.setBacklight(255);                   // LIGA BACKLIGHT
       digitalWrite(LedAM,HIGH);                // Liga Led Chave AMARELA
       digitalWrite(LedVD,HIGH);                // Liga Led Chave VERDE
       digitalWrite(LedVM,HIGH);                // Liga Led Chave VERMELHA
       delay(150);                              // ESPERA 150ms
     } // FIM da Mensagem de Abertura 
                   
    delay(500);                                 // Espera 0,8s e vai para o PROGRAMA PRINCIPAL

    digitalWrite(LedVM, LOW);                   // DESLIGA Led Chave AMARELA
    digitalWrite(LedAM, LOW);                   // DESLIGA Led Chave VERDE
    digitalWrite(LedVD, LOW);                   // DESLIGA Led Chave VERMELHA
    lcd.home(); lcd.clear();
         
}// FIM da CONFIGURAÇÃO DO HARDWARE


//------------------------------------------------------------------------------------------------------

// PROGRAMA PRINCIPAL

void loop(){

// Lê conversores ADCs dos 4 Conectores dos SENSORES para saber quantos e quais tipos de SENSORES estão Instalados 

    for(i=0;i<4; i++)
      {                                         // Chaveia as Portas do Multiplex para Ler os Tipos Dos Sensores Conectados
       digitalWrite(A, HIGH &&(i&B00000001));   // Extrai do número i o Bit A p/ o Multiplex e o escreve na Porta
       digitalWrite(B, HIGH &&(i&B00000010));   // Extrai do número i o Bit B p/ o Multiplex e o escreve na Porta
                                            
                                                         // BA => Multiplex CD4052
                                                         // 00 => Lê Valor do Sensor 1
                                                         // 01 => Lê Valor do Sensor 2
                                                         // 10 => Lê Valor do Sensor 3
                                                         // 11 => Lê Valor do sensor 4
                                            
       sensor[i+1] = analogRead(A0);            // Armazena Valor (TIPO) dos Sensores conectados em sensor[i+1]
                                                // i+1 só p/ facilitar entendimento, sensor 1, sensor 2... sensor 4
  
       delay(200);
    
      } // Fim da Leitura da conversores ADC, Valores dos Sensores

 
 // Escolhe a Função de acordo com o tipo de Sensor Instalado no Conector 1 
 
    if (sensor[1]>900)                            // Se Nenhum Sensor Conectado, Aguarda algum ser Conectado no Conector 1!!!
     {
      if (geral==1)                               // Se já verificou e nenhum sensor foi conectado ainda, pisca ledVM
       { 
        delay(1500);                              // Espera 1,5s para fazer nova pesquisa por algum Sensor Instalado no Conector 1
        digitalWrite(LedVM, !digitalRead(LedVM));
        loop();                                   // Volta para loop(), continuando a esperar algum sensor conectado
       }
        lcd.home(); lcd.clear();                  // Retorna cursor para home e Limpa LCD 
        lcd.print("Conectar Sensor");             // Imprime Mensagem na coluna 1 Linha 1
 
        lcd.setCursor(0,1);                       // Posiciona curso no inicio da Linha 2
        lcd.print("   na Porta 1");               // Imprime Mensagem na Linha 2
        geral=1;
       
      } // Retorna a fazer a leitura do sensor  

      
    else                                          // Sensor CONECTADO!!!! Agora vamos identifica-lo.
       if (sensor[1]>120 && sensor[1]<150)        // Porta 1 (sensor[1]) tem Sensor de Passagem Conectado?
        {                                         // Se SIM, 
         b=1;                                     // Armazena o tipo do Sensor(es) Instalado(s), b=1 ==> PhotoGate
         
         c=1;                                     // E faz c=1 indicando 1 Sensor de Passagem já está conectado

         for(i=2;i<5;i++)                         // Busca Por Mais Sensores de Passagem
           {                  
            if (sensor[i]>120 && sensor[i]<150)   // Há mais Sensores de Passagem nos Conectores 2, 3, 4?
             { 
              a = 3;                              // Com 2 ou mais sensores, Funções Disponiveis: Tempo_passagem e Cronometros
              c = c + 1;                          // Se sim, c=c+1 p/ indicar quantos Sensores de Passagem estão conectados
             }                                    // Volta p/ verificar próximo Conector por outro Sensor de Passagem conectado
           
         else                                     // Se o Sensor não é o de Passagem ou não há mais Sensores instalados,
            {
             break;                               // Pára a busca por mais Sensores 
            }                                       
       } // Busca por Sensores de Passagem FINALIZADA
            
     lcd.home(); lcd.clear();                     // Limpa LCD e posiciona o Cursor em HOME

     lcd.print("  PhotoGate ON");                 // Imprime Mensagem na Linha 1

     lcd.setCursor(2,1);                          // Posiciona cursor na Linha 2
     lcd.print(c);                                // Imprime na Linha 2 o Numero de Sensores Conectados
     lcd.print(" Sensor(es)");
         
     delay(2000);                                 // Espera 3 segundos
     PhotoGate();                                 // Vai Para a Rotina PhotoGate
     }     
            
     if (sensor[1]>230 && sensor[1]<250)          // Porta 1 (sensor[1]) tem Sensor refletivo conectado?
      {                                           // Se Sim,    
       b=2;                                       // Armazena o tipo do Sensor Instalado, b=2 ==> Frequencimetro 
              
       lcd.home(); lcd.clear();                   // Limpa LCD e Posiciona Cursor em HOME
              
       lcd.print(" FREQUENCIMETRO");              // Imprime Mensagem na Linha 1
              
       lcd.setCursor(0,1);                        // Posiciona Cursor na Linha 2
             
       lcd.print("    ATIVADO");                  // Imprime Mensagem na Linha 2
              
       delay(3000);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Rode p/ iniciar");
       
                                      // Espera 3 segundos
              
      //Cronometro();
      Frequencimetro();                          // Vai para Rotina Frequencimetro
      }

                     
        /* FUNÇÕES A SEREM IMPLEMENTADAS        
           
     if (sensor[1]>280 && sensor[1]<310)          // VERIFICAR VALOR RETORNADO PELOS ADCs DOS NOVOS SENSORES,
      {                                           // ADC>(ADC-15) && ADC<(ADC+15) 
       b=3; // Futura Função a ser Implementada
      }                                           //  ==>  ESCOLHER RESISTORES DO SENSOR PARA QUE FIQUE DISTANTE 50 
                                                  //     DO VALOR ANTERIOR MÁXIMO E 50 DO VALOR POSTERIOR MÍNIMO
     if (sensor[1]>150 && sensor[1]<180)
      {
       b=4; // Futura Função a ser Implementada
      }           
     if (sensor[1]<20)
      {
       b=5; // Futura Função a ser Implementada
      }*/

} // FIM DO PROGRAMA PRINCIPAL void loop(), se chegou aqui, Nenhum sensor foi Conectado no conector 1 e volta a pesquisar


//------------------------------------------------------------------------------------------------------------------------

// Rotina para escolher as Funções quando identificado Sensor PhotoGate conectado, de acordo com a quantidade deles

void PhotoGate()
{                                                 // Escolha o tipo de medida que o PhotoGate irá Realizar
while(1)
    {
               
          lcd.home(); lcd.clear();                // Limpa LCD e Posiciona Cursor em Home
          lcd.print("Escolha a Funcao");          // Imprime Mensagem na Linha 1
          lcd.setCursor(0,1);                     // Posicina Cursor na linha 2
                            
          for(i=0; i<3; i++)                      // Loop, Faz Piscar Mensagem da Linha 2, 3 vezes
            {    
             lcd.setCursor(0,1);                  // Posiciona cursor na Linha 2
             lcd.print("                ");       // Apaga a Linha 2         
             delay(200);                          // Espera 300 ms
             lcd.setCursor(0,1);                  // Posiciona Cursor na Linha 2
             lcd.print("<-  VM = OK   ->");       // Imprime Mensagem na Linha 2
             delay(200);                          // Espera 300 ms        
            }                                    
          delay(500);                             // Espera 300 ms      
          //digitalWrite(LedAM, HIGH);              // Liga Led Chave AMARELA   - Indicando que Chave esta Habilitada para receber comando
          //digitalWrite(LedVD, HIGH);              // Liga Led Chave VERDE     - Indicando que Chave esta Habilitada para receber comando
          //digitalWrite(LedVM, HIGH);              // Liga Led Chave VERMELHA  - Indicando que Chave esta Habilitada para receber comando
                
          delay(500);                             // Espera 500ms


     Botao_Seletor:                                   // Escolha das funções, de acordo com os Sensores Instalados
 
           if (c == 1)                                // Se Somente 1 Sensor está Conectado na Conector 1     
            {

                       
             if (digitalRead(BotaoAM) == LOW && a>1)  // Rotina para Menu Anterior 
              {
               digitalWrite(LedVD,HIGH); lcd.setCursor(14,1);lcd.print("->");
               digitalWrite(LedAM, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoAM) == 0);
               digitalWrite(LedAM,HIGH);              // Liga Led Chave AMARELA
               
               a=a-1;                                 // Decrementa Variável Ponteiro da Funções
              }
             
             if(digitalRead(BotaoVD)== LOW && a<=2)   // Rotina para Menu Seguinte 
              {
               digitalWrite(LedAM, HIGH); lcd.setCursor(0,1);lcd.print("<-");
               digitalWrite(LedVD, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoVD) == 0);
               digitalWrite(LedVD,HIGH);              // Liga Led Chave AMARELA
                
               a=a+1;                                 // Incrementa Ponteiro da Variável das Funções
              }
            }

           if (c >=2)                                 // Dois ou Mais PhotoGates Instalados                            
            {
             if(digitalRead(BotaoAM)== LOW && a>3)    // Se BotaoAM Pressionado Volta Menu
              {
               digitalWrite(LedVD,HIGH); lcd.setCursor(14,1);lcd.print("->");
                
               digitalWrite(LedAM, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoAM) == 0);
               digitalWrite(LedAM,HIGH);              // Liga Led Chave AMARELA
                            
               a=a-1;                                 // Decrementa Variavel Ponteiro das Funções
              }
              
             if(digitalRead(BotaoVD)== LOW && a<=3)   // Se BotaoVD Pressionado,Avança Menu 
              {
               
               digitalWrite(LedAM, HIGH); lcd.setCursor(0,1);lcd.print("<-");
                
               digitalWrite(LedVD, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoVD) == 0);
               digitalWrite(LedVD,HIGH);              // Liga Led Chave AMARELA
                  
               a=a+1;                                 // Incrementa Ponteiro da Variável das Funções                              
              }
            }
              
           lcd.home();                                // Posiciona o Cursor LCD em HOME
           lcd.print(funcao[a]);                      // Imprime a Função que está em foco no Menu
           
           if((a==1 && c==1) || (a<=3 && c>=2)){digitalWrite(LedAM,LOW); digitalWrite(LedVD,HIGH); lcd.setCursor(0,1);lcd.print("  ");}
           if((a==3 && c==1) || (a>3  && c>=2)){digitalWrite(LedVD,LOW); digitalWrite(LedAM,HIGH); lcd.setCursor(14,1);lcd.print("  ");}

           
           digitalWrite(LedVM, !digitalRead(LedVM));  // Pisca LedVM,  do BotaoVM
           delay(200);                                // Aguarda 200ms
              
           if(digitalRead(BotaoVM) == LOW)            // Se BotaoVM For Pressionado, Execute a Função em foco
            {
             digitalWrite(LedVM, LOW);                // Apaga LedVM, do BotaoVM
             while(digitalRead(BotaoVM) == LOW);      // BotaoVM continua pressionado? Se Sim Aguarde aqui sua Liberação
             digitalWrite(LedVM, HIGH);               // Liga LedVM,  do BotaoVM
                  
    // Vá para a Função escolhida
            
             if(a == 1)
              { Contador(); }
      
             if(a == 2)
              { Pendulo();  }
           
             if(a == 3)
              { Tempo_passagem(); }
          
             if(a == 4)
              { Cronometro(); }
            }
          
     goto  Botao_Seletor;                             // Se BotaoVM não foi Precionado, Retorna para escolha da Função a ser executada    
    }
           
}  // Fim da Rotina PhotoGate()          


//-------------------------------------------------------------------------------------------------------------

// ROTINA - CONTADOR DE EVENTOS
 
void Contador()
{ 
                       
          digitalWrite(LedAM, LOW);                   // Desabilita LED AM - Não terá função nesta rotina
          digitalWrite(LedVD, LOW);                   // Desabilita LED VD - Não terá função nesta rotina 
          digitalWrite(LedVM, LOW);                   // Desliga LED VM pois o Botão VM - OK, foi Pressionado
               
          while(digitalRead(BotaoVM) == LOW);         // Botão VM - continua pressionado? Fique aqui
               
          countSum = 0;                               // Zera variável do Contador
               
          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
          lcd.print("Contador Eventos");              // Imprime Menssagem na Linha 1
              
          lcd.setCursor(0,1);                         // Posiciona Cursor para Linha 2
          lcd.print("START - press VM");              // Imprime Menssagem na Linha 2
               
          digitalWrite(LedVM, HIGH);                  // Liga LED VM Botão VM
          delay(1000);                                // Espera 1s com a Menssagem no LCD 

          while(digitalRead(BotaoVM) == HIGH)
              { digitalWrite(LedVM, !digitalRead(LedVM)); delay(500); } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
               
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM

          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
          lcd.print("Contador -> ON");                // Imprime Menssagem na Linha 1
               
          lcd.setCursor(0,1);                         // Possiciona cursor na Linha 2
          lcd.print("Contagem = ");                   // Imprime Menssagem na Linha 2
 
while(1)
    {
     lcd.setCursor(11,1);                             // Possiciona Cursor Para Imprimir valor da Contagem na linha 2  
     lcd.print(countSum);                             // Imprime Valor Contagem Linha 2
     

     desliga_count:                                   // Verifica se o Botão VM foi Pressionado para Finalizar a Contagem

           if(digitalRead(BotaoVM) == LOW)            // Finalizar Contagem? Sim, continua abaixo. Não, vai verificar se houve contagem
            {   
             digitalWrite(LedVM, LOW);                // Apaga LED VM, Pois Botão VM foi Pressionado
             lcd.home();                              // Posiciona Cursor LCD na Coluna 0 da Linha 1
             lcd.print("Contador -> OFF");            // Imprime Mensagem no LCD Linha 1
             while(digitalRead(BotaoVM) == LOW);      // Botão VM Continua Pressionado? Sim, Fique aqui até que seja liberdo
             delay(1000);                             // Espera 1s
             lcd.home();                              // Posiciona cursor coluna 1, Linha 1 
             lcd.print(" VM - Reiniciar ");           // Imprime Mensagem Linha 1  
             while(digitalRead(BotaoVM) == HIGH) 
                 { digitalWrite(LedVM, !digitalRead(LedVM)); delay(100); } // Enquanto Botao VM não for Pressionado para Reiniciar, Pisca LED VM do Botao VM
             Contador();                              // Botão VM foi Pressionado, então Recomece o CONTADOR!!     
            }
               
           if (digitalRead(S1) == HIGH)               // Houve interrução no SENSOR 1? Sim, Incrementa Variavél Contador
            { 
             digitalWrite(LedVM, HIGH);               // SENSOR SEM LUZ, Objeto Passando. Liga LED VM
             countSum ++;                             // Incrementa Variável Contador
                    
             while(digitalRead(S1) == HIGH)           // SENSOR ainda Ativado? Sim, Então até objeto passar, Verifique se Usuário quer Finalizar a Contagem 
                 { 
                  if(digitalRead(BotaoVM) == LOW)     // BotãoVM foi pressionado para Finalizar a contagem?
                   {goto desliga_count; }             // Se SIM, vai para a rotina de Finalizar a contagem
                 }                                    // Se não, volta a verificar se o já Objeto Passou
                     
             digitalWrite(LedVM, LOW);                // Objeto Passou pelo sensor, então desliga LED VM indicando esse evento.
            }
    }                                                 // Volta para verificar próximo evento para contar.
} // Fim Rotina Contador()

// -----------------------------------------------------------------------------------------------------------------------

// Rotina Pêndulo()

void Pendulo()
{

          digitalWrite(LedAM, LOW);                   // Desabilita LED AM - Não terá função nesta rotina
          digitalWrite(LedVD, LOW);                   // Desabilita LED VD - Não terá função nesta rotina 
          digitalWrite(LedVM, LOW);                   // Desliga LED VM pois o Botão VM - OK, foi Pressionado
               
          while(digitalRead(BotaoVM) == LOW);         // Botão VM - continua pressionado? Fique aqui
               
          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
          lcd.print("PENDULO  MEDIA 5");              // Imprime Menssagem na Linha 1
              
          lcd.setCursor(0,1);                         // Posiciona Cursor para Linha 2
          lcd.print("START - press VM");              // Imprime Menssagem na Linha 2
               
          digitalWrite(LedVM, HIGH);                  // Liga LED VM Botão VM
          delay(1000);                                // Espera 1s com a Menssagem no LCD 

          while(digitalRead(BotaoVM) == HIGH)
              { digitalWrite(LedVM, !digitalRead(LedVM)); delay(500); } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
               
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM

          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
          lcd.print("MEDIDO ms  MEDIA");              // Imprime Menssagem na Linha 1

          lcd.setCursor(0,1);                         // Possiciona cursor na Linha 2
          lcd.print("Inicie o Pendulo");              // Imprime Menssagem na Linha 2
          delay(1000);
          lcd.setCursor(0,1);                         // Possiciona cursor na Linha 2
          lcd.print("                ");              // Imprime Menssagem na Linha 2

 
while(1)
    {
     desliga_count1:                                  // Verifica se o Botão VM foi Pressionado para Finalizar a Contagem

     if(digitalRead(BotaoVM) == LOW)                  // Finalizar Contagem? Sim, continua abaixo. Não, vai verificar se houve contagem
      {   
       digitalWrite(LedVM, LOW);                      // Apaga LED VM, Pois Botão VM foi Pressionado
       lcd.home();                                    // Posiciona Cursor LCD na Coluna 0 da Linha 1
       lcd.print(" Pendulo -> OFF ");                 // Imprime Mensagem no LCD Linha 1
       while(digitalRead(BotaoVM) == LOW);            // Botão VM Continua Pressionado? Sim, Fique aqui até que seja liberdo
       delay(1000);                                   // Espera 1s
       lcd.home();                                    // Posiciona cursor coluna 1, Linha 1 
       lcd.print(" VM - Reiniciar ");                 // Imprime Mensagem Linha 1  
       while(digitalRead(BotaoVM) == HIGH)
           { digitalWrite(LedVM, !digitalRead(LedVM)); delay(100); } // Enquanto Botao VM não for Pressionado para Reiniciar, Pisca LED VM do Botao VM
                                          
       Pendulo();                                     // Botão VM foi Pressionado, então Recomece o CONTADOR!!     
      }
 
     Periodo_medio=0;                                 // Zera Valor Periodo_medio, para uma nova Medida
         
      for(i=0; i<5; i++)
        {
         // Primeira Passagem, Inicia Medida do Periodo de Oscilação
 
         while(digitalRead(S1) == LOW)                // Houve 1a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar 
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
                                 
         digitalWrite(LedVD,HIGH);                    // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
        
         while(digitalRead(S1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem do Objeto
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }

         Tempo_inicial=Tempo_final;
         if(i==0){Tempo_inicial=millis();}                      // Anota o Tempo no momento inicial da medida
                                                 
         digitalWrite(LedVD,  LOW);                   // Apaga LedVD, para indicar que o Objeto Passou
        
         digitalWrite(LedVM, HIGH);                   // Liga LedVM para indicar que Iniciou a Medida do Periodo de Oscilação
    
         // Segunda Passagem, Ignora essa Medida pois queremos o Periodo de Oscilação Completo

         if(i==0)
         {
          lcd.setCursor(0,1);                          // Posiciona cursor na Linha 2
          lcd.print("T");                              // Imprime Menssagem na Linha 2
          lcd.print(i+1);                              // Imprime qual Periodo esta sendo medido 1 a 5
          lcd.print("=             ");                 // Imprime "="
          
          
         while(digitalRead(S1) == LOW)                // Houve 2a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
            
         digitalWrite(LedVD, HIGH);                   // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
    
         while(digitalRead(S1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
             
         digitalWrite(LedVD, LOW);                    // Apaga LedVD, para indicar que o Objeto Passou      
         }
         // Terceira Passagem, Finaliza a Medida do Periodo de Oscilaçao

         while(digitalRead(S1) == LOW)                // Houve 1a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar 
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
                                 
         digitalWrite(LedVD, HIGH);                   // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
        
         while(digitalRead(S1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem do Objeto
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }

         Tempo_final=millis();                               
 
         digitalWrite(LedVD,  LOW);                   // Apaga LedVD, para indicar que o Objeto Passou
                                
         Periodo[i] = Tempo_final - Tempo_inicial;    // Calcula o Periodo da Oscilação e o armazena na Matriz

         digitalWrite(LedVM,  LOW);                   // Desliga LedVM para indicar que Finalizou a Medida do Periodo de Oscilação
                      
         lcd.setCursor(0,1);                          // Posiciona cursor na Linha 2
         lcd.print("T");                              // Imprime Menssagem na Linha 2
         lcd.print(i+1);                              // Imprime qual Periodo esta sendo medido 1 a 5
         lcd.print("=");                              // Imprime "="
         lcd.print(Periodo[i],0);                       // Imprime Valor do Periodo atual Medido
         lcd.print("   ");                            // Limpa sujeiras de Mensagens anteriores
         Periodo_medio=Periodo_medio+Periodo[i];      // Soma os Valores dos Periodos Coletados para calcular a Média
    
    
        }// Fim da Coleta dos 5 Periodos
    
      lcd.setCursor(9,1);                            // Posiciona Cursor para Linha 2
      lcd.print(Periodo_medio/5,2);                   // Calcula a Média dos 5 Periodos Coletados e Imprime no LCD

     // Ignora uma passagem para recomeçar do mesmo ponto de lançamento
      while(digitalRead(S1) == LOW)                // Houve 2a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
            
         digitalWrite(LedVD, HIGH);                   // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
    
         while(digitalRead(S1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
             
         digitalWrite(LedVD, LOW);                    // Apaga LedVD, para indicar que o Objeto Passou      
         
   
    }// Volta a fazer uma nova Colera de Periodos, até BotaoVM - Finalizar - ser Pressionado

}// Fim Rotina Pendulo()


//-------------------------------------------------------------------------------------------------------------------------

// Rotina Tempo_passagem()

void Tempo_passagem()
{

          digitalWrite(LedAM, LOW);                   // Desabilita LED AM - Não terá função nesta rotina
          digitalWrite(LedVD, LOW);                   // Desabilita LED VD - Não terá função nesta rotina 
          digitalWrite(LedVM, LOW);                   // Desliga LED VM pois o Botão VM - OK, foi Pressionado
               
          while(digitalRead(BotaoVM) == LOW);         // Botão VM - continua pressionado? Fique aqui

             
          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
         
          if (c == 1)
          {
          lcd.print("t-Pass   MEDIA 5");              // Imprime Menssagem na Linha 1
          }

          if (c >= 2)
          {
            lcd.print("Temp de Passagem");
          }
          
          lcd.setCursor(0,1);                         // Posiciona Cursor para Linha 2
          lcd.print("START - press VM");              // Imprime Menssagem na Linha 2
               
          digitalWrite(LedVM, HIGH);                  // Liga LED VM Botão VM

          while(digitalRead(BotaoVM) == HIGH)
              { digitalWrite(LedVM, !digitalRead(LedVM)); delay(500); } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
                    
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
                 
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM

          lcd.home(); lcd.clear();                    // Limpa LCD e Posiciona em HOME
          
          if ( c == 1)
          {
            lcd.print("MEDIDO ms  MEDIA");              // Imprime Menssagem na Linha 1
          }

          if ( c >= 2)
          {
            lcd.print("  Tempos em ms  "); 
          }
          lcd.setCursor(0,1);                         // Possiciona cursor na Linha 2
          lcd.print("Solte Corpo Test");              // Imprime Menssagem na Linha 2
          delay(1000);
          lcd.setCursor(0,1);                         // Possiciona cursor na Linha 2
          lcd.print("                ");              // Imprime Menssagem na Linha 2

 
while(1)
    {
     desliga_count1:                                  // Verifica se o Botão VM foi Pressionado para Finalizar a Contagem

     if(digitalRead(BotaoVM) == LOW)                  // Finalizar Contagem? Sim, continua abaixo. Não, vai verificar se houve contagem
      {  
       
       digitalWrite(LedVM, LOW);                      // Apaga LED VM, Pois Botão VM foi Pressionado
       lcd.home();                                    // Posiciona Cursor LCD na Coluna 0 da Linha 1
       lcd.print(" t-Pass --> OFF ");                 // Imprime Mensagem no LCD Linha 1
       while(digitalRead(BotaoVM) == LOW);            // Botão VM Continua Pressionado? Sim, Fique aqui até que seja liberdo
       delay(1000);                                   // Espera 1s
       lcd.home();                                    // Posiciona cursor coluna 1, Linha 1 
       lcd.print(" VM - Reiniciar ");                 // Imprime Mensagem Linha 1  
       while(digitalRead(BotaoVM) == HIGH)
           { digitalWrite(LedVM, !digitalRead(LedVM)); delay(100); } // Enquanto Botao VM não for Pressionado para Reiniciar, Pisca LED VM do Botao VM
                                  
      Tempo_passagem();                                     // Botão VM foi Pressionado, então Recomece o CONTADOR!!     
      }
 
     Periodo_medio=0;                                 // Zera Valor Periodo_medio, para uma nova Medida
         
      
      if ( c == 1)
      {
      for(i=0; i<5; i++)
        {
         // Inicia Medida do Tempo de passagem do Corpo de Teste e faz a Média
 
         while(digitalRead(S1) == LOW)                // Houve 1a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar 
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {
               goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
         
         Tempo_inicial=micros();                      // Anota o Tempo no momento inicial da medida                        
         
         digitalWrite(LedVD,HIGH);                    // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
        
         while(digitalRead(S1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem do Objeto
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {
                goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }

         Tempo_final=micros();                        // Anota o Tempo no momento final da medida
                                                 
         digitalWrite(LedVD,  LOW);                   // Apaga LedVD, para indicar que o Objeto Passou
                          
         Periodo[i] = ((Tempo_final - Tempo_inicial)*0.001);    // Calcula o Periodo da Oscilação e o armazena na Matriz

                      
         lcd.setCursor(0,1);                          // Posiciona cursor na Linha 2
         lcd.print("T");                              // Imprime Menssagem na Linha 2
         lcd.print(i+1);                              // Imprime qual Periodo esta sendo medido 1 a 5
         lcd.print("=");                              // Imprime "="
         lcd.print(Periodo[i],2);                       // Imprime Valor do Periodo atual Medido
         lcd.print("   ");                            // Limpa sujeiras de Mensagens anteriores
         Periodo_medio=Periodo_medio+Periodo[i];      // Soma os Valores dos Periodos Coletados para calcular a Média

        if (i<4)
        {
          while(digitalRead(BotaoVM) == HIGH)
          { digitalWrite(LedVM, !digitalRead(LedVM)); delay(500); } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
          
          delay(100); // Delay Debounce 
          
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
          
          delay(100); // Delay Debounce 
          
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM
    
        }// Fim da Coleta dos 5 Periodos
   }
      lcd.setCursor(10,1);                            // Posiciona Cursor para Linha 2
      lcd.print(Periodo_medio/5,2);                   // Calcula a Média dos 5 Periodos Coletados e Imprime no LCD
      
      
      while(digitalRead(BotaoVM) == HIGH)
        { digitalWrite(LedVM, !digitalRead(LedVM)); } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
            
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM
   
    }// Volta a fazer uma nova Colera de Periodos, até BotaoVM - Finalizar - ser Pressionado
    
    if ( c >=2)
    {
      for(i=1; i<=c; i++)
        {
         // Inicia Medida do Tempo de passagem do Corpo de Teste e faz a Média
 
         while(digitalRead(i+1) == LOW)                // Houve 1a. passagem pelo Sensor? Se não, aguarda aqui até Objeto passar 
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {
               goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }
         
         Tempo_inicial=micros();                      // Anota o Tempo no momento inicial da medida                        
         
         digitalWrite(LedVD,HIGH);                    // Liga LedVD, para indicar que Objeto está Passando pelo Sensor
        
         while(digitalRead(i+1) == HIGH)               // O objeto ainda esta Passando? Se sim, aguarda finalizar a passagem do Objeto
             {if(digitalRead(BotaoVM) == LOW)         // Enquanto aguarda, foi pressionado BotaoVM para Finalizar a contagem?
               {
                goto desliga_count1;}                 // Se SIM, vai para a rotina de Finalizar a Medida                             
             }

         Tempo_final=micros();                        // Anota o Tempo no momento final da medida
                                                 
         digitalWrite(LedVD,  LOW);                   // Apaga LedVD, para indicar que o Objeto Passou
                          
         Periodo[i] = ((Tempo_final - Tempo_inicial)*0.001);    // Calcula o Periodo da Oscilação e o armazena na Matriz

                      
         lcd.setCursor(0,1);                          // Posiciona cursor na Linha 2
         lcd.print("T");                              // Imprime Menssagem na Linha 2
         lcd.print(i);                              // Imprime qual Periodo esta sendo medido 1 a 5
         lcd.print("=");                              // Imprime "="
         lcd.print(Periodo[i],2);                       // Imprime Valor do Periodo atual Medido
         lcd.print("   ");                            // Limpa sujeiras de Mensagens anteriores
        
    
        }// Fim da Coleta dos sensores  
         
         geral=i-1; i=1;
         
         lcd.setCursor(0,0); lcd.print("<- ver tempos ->");
          
           while(digitalRead(BotaoVM))
           { 
           if (digitalRead(BotaoAM) == LOW && geral>1)  // Rotina para Menu Anterior 
              {
               digitalWrite(LedVD,HIGH); lcd.setCursor(14,0);lcd.print("->");
               digitalWrite(LedAM, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoAM) == 0);
               digitalWrite(LedAM,HIGH);              // Liga Led Chave AMARELA
               
               geral=geral-1;                                 // Decrementa Variável Ponteiro da Funções
              i=1;
              }
             
             if(digitalRead(BotaoVD)== LOW && geral<c)   // Rotina para Menu Seguinte 
              {
               digitalWrite(LedAM, HIGH); lcd.setCursor(0,0);lcd.print("<-");
               digitalWrite(LedVD, LOW);              // Desliga Led Chave AMARELA
               while(digitalRead(BotaoVD) == 0);
               digitalWrite(LedVD,HIGH);              // Liga Led Chave AMARELA
                
               geral=geral+1;                                 // Incrementa Ponteiro da Variável das Funções
              i=1;
              }

              if(i==1)
              {
              lcd.setCursor(0,1);lcd.print("T");lcd.print(geral);lcd.print("=           ms");lcd.setCursor(3,1);lcd.print(Periodo[geral]);
              i=0;
              }
           if(geral == 1){digitalWrite(LedAM,LOW); digitalWrite(LedVD,HIGH); lcd.setCursor(0,0);lcd.print("  ");}
           if(geral == c){digitalWrite(LedVD,LOW); digitalWrite(LedAM,HIGH); lcd.setCursor(14,0);lcd.print("  ");}

           }
           
      
      while(digitalRead(BotaoVM) == HIGH)
        { 
          digitalWrite(LedVM, !digitalRead(LedVM));
          
          } // Botão VM Pressionado? Não, Pisca LED VM Botão VM
            
          digitalWrite(LedVM, LOW);                   // Botão VM Pressionado, Desliga LED VM Botao VM
          while(digitalRead(BotaoVM) == LOW);         // Botão VM Continua Pressionado? Sim, Fique aqui
          digitalWrite(LedVM, HIGH);                  // Botão VM Liberado, acende LED VM
   
    }// Volta a fazer uma nova Colera de Periodos, até BotaoVM - Finalizar - ser Pressionado
    
    }
}// Fim Rotina Tempo de Passagem()


//-------------------------------------------------------------------------------------------------------------------------

// Rotina Cronometro

void Cronometro()
{

               
         lcd.home(); lcd.clear();
         lcd.print("   Cronometro   ");
         lcd.setCursor(0,1);
         lcd.print("    Simples     ");
         
         delay(2000);
        
        while(1)
            {
  
              if(x==0)
                {
                  lcd.clear();
                  if(x==0){lcd.print("Press VM ->start");}
                  x=1;
                 }

  
               if(digitalRead(BotaoVM) == LOW)
                 {

                   lcd.clear();
                   k = millis();
                    lcd.setCursor(9,0);
                    lcd.print("Segund.");
                        lcd.setCursor(0,1);
                        lcd.print("   VD ->    STOP");
                   
                  while(digitalRead(BotaoVD) == HIGH)
                      {
   
                        p = millis();
                        j = (p - k) / 1000;
                        lcd.setCursor(0,0);
                        lcd.print(j);
                       
  
                       // delay(100);
                      }
   
                if(digitalRead(BotaoVD) == LOW)
                  {
                    while(digitalRead(BotaoVM) == HIGH)
                        {
                          lcd.setCursor(0,0);
                          lcd.print(j);
                          lcd.setCursor(11,0);
                          lcd.print("");
                          lcd.setCursor(0,1);
                          lcd.print("   VM -> RESTART");       
                        }
                   }
          }
      }

}

//-------------------------------------------------------------------------------------------------------------------------

// Rotina Frequencimetro

void Frequencimetro(){
  
while(digitalRead(S1) == HIGH){}
        if(digitalRead(S1) == LOW){
          while(digitalRead(S1) != HIGH){}
        }

          t1 = millis();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Periodo");
          digitalWrite(LedVM, HIGH);
      
          for(i = 0; i < Nmedidas; i++){

            
            if(digitalRead(S1) == LOW){
              while(digitalRead(S1) != HIGH){}
            }
            
            t0 = t1;
            t1 = millis();
      
            T = t1 - t0;
            lcd.setCursor(0,1);
            lcd.print(T);
            if (i==15){
              lcd.setCursor(9,0);
              lcd.print("Solte");
              digitalWrite(LedVM, LOW);
            }
            //Serial.println(T);
            medidas[i] = T;
            delay(10);
            //lcd.write(0);
           
          }
          
        t0 = 0;
        t1 = 0;

      

  AD = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Periodos ms ");
  redstate=HIGH;

  while(redstate == HIGH){
  redstate = digitalRead(BotaoVM);
  greenstate = digitalRead(BotaoVD);
  lrstate = digitalRead(BotaoAM);

  if(lrstate==LOW){
    nav=nav-1;
    if (nav<=0){
      nav=0;
    }
    lcd.setCursor(0,1);
    //lcd.write(0);
    lcd.print(medidas[nav]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    //lcd.write(0);
    lcd.print(nav);
    lrstate=HIGH;
    delay(1000);
  }

  else if(greenstate ==LOW){
    nav=nav+1;
    if (nav>=30){
      nav=0;
    }
    lcd.setCursor(0,1);
    //lcd.write(0);
    lcd.print(medidas[nav]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    //lcd.write(0);
    lcd.print(nav);
    greenstate=HIGH;
    delay(1000);
    
  }
  else{
    lcd.setCursor(0,1);
    //lcd.write(0);
    lcd.print(medidas[nav]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    //lcd.write(0);
    lcd.print(nav);
  }
  }
}

