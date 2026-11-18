#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int soilPin = A0;    
const int waterPin = A1;   
const int relayPin = 7;    

const int soilDryThreshold = 500;  
const int waterLowThreshold = 200; 

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); 
  
  lcd.begin(16, 2);
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  int soilValue = analogRead(soilPin);
  int waterValue = analogRead(waterPin);
  
  int soilPercent = map(soilValue, 1023, 0, 0, 100);
  int waterPercent = map(waterValue, 0, 700, 0, 100);
  waterPercent = constrain(waterPercent, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilPercent);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterPercent);
  lcd.print("%   ");

  if (soilValue > soilDryThreshold && waterValue > waterLowThreshold) {
    digitalWrite(relayPin, LOW);  
  } else {
    digitalWrite(relayPin, HIGH); 
  }

  delay(1000);
}