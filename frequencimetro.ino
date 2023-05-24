#define Nmedidas 10
#define AnalogSensor 0
#define SensorPin 2
#define SoltarPin 8

unsigned int t0 = 0;
unsigned int t1 = 0;
unsigned int T = 0;
int i = 0;
int AD = 0;
int transicao = 0;
int tr0 = 0;


unsigned int medidas[10];






void setup() {


  pinMode(SensorPin, INPUT);
//  pinMode(SoltarPin, INPUT);


  Serial.begin(9600);

  Serial.println("Conecte um sensor para começar!");

}



void loop() {

  while(AD != 240) AD = analogRead(AnalogSensor);

  //Identificando o sensor
      // Frequencímetro
        while(digitalRead(SensorPin) == HIGH){}
        if(digitalRead(SensorPin) == LOW){
          while(digitalRead(SensorPin) != HIGH){}
        }
        //Começa as medidas

          t1 = millis();
      
          for(i = 0; i < Nmedidas; i++){

            /*
             if(i == 5){
              Serial.println("Solte!");
              while(soltar == LOW) soltar = digitalRead(SoltarPin);   
              //delay(TempoDeQueda);
              t1 = millis();
            } */ 
            if(digitalRead(SensorPin) == LOW){
              while(digitalRead(SensorPin) != HIGH){}
            }
            
            t0 = t1;
            t1 = millis();
      
            T = t1 - t0;
            
            Serial.println(T);
            medidas[i] = T;
            delay(10);
           
          }
          
        t0 = 0;
        t1 = 0;

      

  AD = 0;
  

}
