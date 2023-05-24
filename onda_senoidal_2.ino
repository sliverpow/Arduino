#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
int i;
int k;
int j=0;
String s;
const float pi = 3.14;

byte customChar8[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte customChar7[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte customChar6[8] = {
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte customChar5[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte customChar4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte customChar3[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111
};
byte customChar2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};
byte customChar1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
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

byte jj11[] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte jj12[] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte jj13[] = {
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte jj14[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
byte jj15[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};
byte jj16[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000
};
byte jj17[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};
byte jj18[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
void setup() {
  lcd.init();
  lcd.backlight();
  //lcd.createChar(0, customChar0);
  lcd.createChar(0, customChar1);
  lcd.createChar(1, customChar2);
  lcd.createChar(2, customChar3);
  lcd.createChar(3, customChar4);
  lcd.createChar(4, customChar5);
  lcd.createChar(5, customChar6);
  lcd.createChar(6, customChar7);
  lcd.createChar(7, customChar8);
  //lcd.createChar(11, jj11);
  //lcd.createChar(12, jj12);
  //lcd.createChar(13, jj13);
  //lcd.createChar(14, jj14);
  //lcd.createChar(15, jj15);
  //lcd.createChar(16, jj16);
  //lcd.createChar(17, jj17);
  //lcd.createChar(18, jj18);
  lcd.clear();
  lcd.setBacklight(HIGH);
}

void loop() {
lcd.setBacklight(HIGH);for (i=0;i<=180;i=i+15){
k=sin(i*pi/180.0)*8;
if (k>=0){
  Serial.println(j);
  lcd.setCursor(j,1);
  lcd.write(k);
}
//if(k<0){
  //k=k*(-1);
  //Serial.println(10+k);
  //lcd.setCursor(j,0);
  //lcd.write(10+k);

j=j+1;
delay(40);

if (j>15){
  j=0;
}
}
   }
