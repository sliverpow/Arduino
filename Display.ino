#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Nmedidas 10
#define AnalogSensor 0
#define SensorPin 2
#define SoltarPin 8

#define redbuton  10
#define ledred  13

#define greenbuton  9
#define ledgreen 12

#define lrbuton  8
#define ledlr 11



int redstate = HIGH ;
int greenstate = HIGH ;
int lrstate = HIGH ;


unsigned int t0 = 0;
unsigned int t1 = 0;
unsigned int T = 0;
int i = 0;
int k=0;
int AD = 0;
int transicao = 0;
int tr0 = 0;

int medidas[10];

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


void setup() {
  lcd.setBacklight(HIGH);


  pinMode(SensorPin, INPUT);
  pinMode(redbuton,INPUT_PULLUP);
  pinMode(greenbuton,INPUT_PULLUP);
  pinMode(lrbuton,INPUT_PULLUP);
  Serial.begin(9600);

  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("LAB-Micro");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Conecte o Sensor");
  delay(1000);

  while(AD != 240){ 
  AD = analogRead(AnalogSensor);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Frenquencimetro");
  delay(2000);
  lcd.createChar(0, customChar0);


}

void loop() {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Red to Start");
  delay(3000);

  while(redstate == HIGH){
  redstate = digitalRead(redbuton);
  }

  lcd.clear();



  lcd.clear();
  while(digitalRead(SensorPin) == HIGH){}
        if(digitalRead(SensorPin) == LOW){
          while(digitalRead(SensorPin) != HIGH){}
        }
        //Começa as medidas
          lcd.setCursor(0,0);
          lcd.print("Periodo");
          t1 = millis();
      
          for(i = 0; i < Nmedidas; i++){

            
            if(digitalRead(SensorPin) == LOW){
              while(digitalRead(SensorPin) != HIGH){}
            }
            
            t0 = t1;
            t1 = millis();
      
            T = t1 - t0;
            lcd.setCursor(0,1);
            lcd.print(T);
            Serial.println(T);
            medidas[i] = T;
            delay(10);
            lcd.write(0);
           
          }
          
        t0 = 0;
        t1 = 0;

      

  AD = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Periodos");
  redstate=HIGH;

  while(redstate == HIGH){
  redstate = digitalRead(redbuton);
  greenstate = digitalRead(greenbuton);
  lrstate = digitalRead(lrbuton);

  if(lrstate==LOW){
    k=k-1;
    if (k<=0){
      k=0;
    }
    lcd.setCursor(0,1);
    lcd.write(0);
    lcd.print(medidas[k]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    lcd.write(0);
    lcd.print(k);
    greenstate=HIGH;
  }

  else if(greenstate ==LOW){
    k=k+1;
    if (k>=10){
      k=0;
    }
    lcd.setCursor(0,1);
    lcd.write(0);
    lcd.print(medidas[k]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    lcd.write(0);
    lcd.print(k);
    lrstate=HIGH;
    delay(1000);
    
  }
  else{
    lcd.setCursor(0,1);
    lcd.write(0);
    lcd.print(medidas[k]);
    lcd.setCursor(7,1);
    lcd.print("Medida");
    lcd.setCursor(13,1);
    lcd.write(0);
    lcd.print(k);
  }
  }







}
