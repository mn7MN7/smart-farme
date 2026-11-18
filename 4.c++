#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- 1. إعدادات شاشة LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // جرب 0x3F إذا لم تظهر الكتابة

// --- 2. إعدادات قارئ الـ RFID ---
#define RST_PIN 9   
#define SS_PIN 10   
MFRC522 mfrc522(SS_PIN, RST_PIN);

// --- 3. إعدادات محركات السيرفو ---
Servo servo1;
Servo servo2;
const int servo1Pin = 3; 
const int servo2Pin = 5; 

// --- 4. إعدادات لوحة المفاتيح (Keypad 4x4) ---
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {2, 4, 6, 7}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- المتغيرات والثوابت ---
String secretCode = "1234"; // الرمز السري (يمكنك تغييره)
String inputCode = "";

// ضع الـ UID الخاص بكارتك هنا بعد فحصه
byte authorizedUID[] = {0xDE, 0xAD, 0xBE, 0xEF}; 

void setup() {
  Serial.begin(9600);
  SPI.begin();          
  mfrc522.PCD_Init();   
  
  // تشغيل شاشة الـ LCD
  lcd.init();                      
  lcd.backlight();
  
  // ربط السيرفو بالبنات
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  
  // الوضع الافتراضي عند الإقلاع (الأبواب مغلقة)
  lockDoors();
  updateDisplay("System Ready", "Enter Code:");
}

void loop() {
  // 1. استقبال الرمز من لوحة المفاتيح
  char key = keypad.getKey();
  if (key) {
    if (key == '#') { // زر التأكيد
      if (inputCode == secretCode) {
        updateDisplay("Correct Code!", "Doors: OPEN");
        openDoors();
        delay(3000); // تظل مفتوحة لمدة 3 ثوانٍ
        lockDoors();
      } else {
        updateDisplay("Wrong Code!", "Doors: LOCKED");
        delay(2000);
        updateDisplay("Enter Code:", "");
      }
      inputCode = ""; 
    } 
    else if (key == '*') { // زر التصفير / الحذف
      inputCode = "";
      updateDisplay("Enter Code:", "");
    } 
    else {
      inputCode += key;
      updateDisplay("Enter Code:", inputCode);
    }
  }

  // 2. التحقق من قراءة كارت الـ RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (checkCardUID()) {
      updateDisplay("Card Accepted", "Doors: OPEN");
      openDoors();
      delay(3000);
      lockDoors();
    } else {
      updateDisplay("Wrong Card!", "Doors: LOCKED");
      delay(2000);
      updateDisplay("Enter Code:", "");
    }
    mfrc522.PICC_HaltA(); 
  }
}

// --- الدوال الفرعية ---

void openDoors() {
  servo1.write(90); // زاوية الفتح
  servo2.write(90);
}

void lockDoors() {
  servo1.write(0);  // زاوية الإغلاق
  servo2.write(0);
  updateDisplay("Enter Code:", ""); 
}

boolean checkCardUID() {
  if (mfrc522.uid.size != 4) return false;
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }
  return true;
}

void updateDisplay(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}