#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 2 //Porta a qual o cabo de comunicação está conectado
#define TEMPERATURE_PRECISION 9



OneWire oneWire(ONE_WIRE_BUS);

//definindo uma variável para os sensores
DallasTemperature sensors(&oneWire);
DeviceAddress Sensor1, Sensor2;


unsigned long t0;
unsigned int N_medidas = 0;
unsigned int N_medias = 0;
unsigned int i, j;

float tempC1, tempC2, tempC1_avg, tempC2_avg;

char command = 'F';
bool C_or_F = false;




void setup() {
  Serial.begin(9600);
  sensors.begin();
// Encontrar o endereço dos sensores
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");


  if (!sensors.getAddress(Sensor1, 0)) Serial.println("Não foi possível encontrar o endereço para o primeiro sensor.");
  else {
    Serial.print("Primeiro sensor encontrado com endereço ");
    printAddress(Sensor2);
    Serial.println();
  }
  if (!sensors.getAddress(Sensor2, 1)) Serial.println("Não foi possível encontrar o endereço para o segundo sensor.");
  else {
    Serial.print("Segundo sensor encontrado com endereço ");
    printAddress(Sensor2);
    Serial.println();
  }

  tempC1_avg = 0.0;
  tempC2_avg = 0.0;
  
}

void loop() {

    command = Serial.read();

  //switch que faz o envio de dados e recebe os parametros do programa caso enviados
    switch(command){
  
      //Plot do sensor 1
      case 'A':
  
        t0 = millis();
      
        for(i = 0; i < N_medidas; i++){ 
          
          for(j = 0; j < N_medias; j++){
            tempC1 = sensors.getTempC(Sensor1);
            if (tempC1 == DEVICE_DISCONNECTED_C) Serial.println("Erro: Não foi possível ler a temperatura.");
            tempC1_avg += tempC1;
          }
  
          tempC1_avg = tempC1_avg/N_medias;
           
          printTemperature(tempC1_avg, t0, C_or_F);
          Serial.println();
          
          tempC1_avg = 0;
          
        }
  
      //Plot do sensor 2
      case 'B':
  
        t0 = millis();
  
        for(i = 0; i < N_medidas; i++){ 
            
            for(j = 0; j < N_medias; j++){
              tempC2 = sensors.getTempC(Sensor2);
              if (tempC2 == DEVICE_DISCONNECTED_C) Serial.println("Erro: Não foi possível ler a temperatura.");
              tempC2_avg += tempC2;
            }
    
            tempC2_avg = tempC2_avg/N_medias;
             
            printTemperature(tempC2_avg, t0, C_or_F);
            Serial.println();
            
            tempC2_avg = 0; 
        }
  
  
      //Plot dos sensores 1 e 2
      case 'C':
  
        t0 = millis();
  
        for(i = 0; i < N_medidas; i++){ 
            
            for(j = 0; j < N_medias; j++){
              tempC1 = sensors.getTempC(Sensor2);
              tempC2 = sensors.getTempC(Sensor2);
              if (tempC1 == DEVICE_DISCONNECTED_C || tempC2 == DEVICE_DISCONNECTED_C) Serial.println("Erro: Não foi possível ler a temperatura.");
              tempC1_avg += tempC1;
              tempC2_avg += tempC2;
            }
  
            tempC1_avg = tempC1_avg/N_medias;
            tempC2_avg = tempC2_avg/N_medias;
  
            printTemperature(tempC1_avg, t0, C_or_F);
            printTemperature(tempC2_avg, t0, C_or_F);
            Serial.println();
            
            tempC1_avg = 0;
            tempC2_avg = 0; 
        }
  
        
      //Apagar
      case 'D':
  

      //Coleta de dados
      case 'E':
  
        N_medidas = Serial.parseInt();
        N_medias = Serial.parseInt();
        C_or_F = Serial.parseInt();


  }

  command = 'F';
}


//função que definitivamente imprime os valores dos sensores
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // adicionando zeros ao endereço se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


void printTemperature(float tempC, unsigned long tempo0, bool CorF)
{

  //Verifica se o usuário pediu a medição em celsius ou farenheit. C = false, F = true 
  if(CorF == false){
    Serial.print(millis() - tempo0);
    Serial.print(",");
    Serial.print(tempC);
  }
  else{
    Serial.print(millis() - tempo0);
    Serial.print(",");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
}
