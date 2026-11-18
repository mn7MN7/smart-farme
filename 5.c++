#include <Servo.h>

Servo myServo;

const int buttonPin = 2;

void setup() {
  myServo.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);

  myServo.write(0);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {

    myServo.write(90);  // يتحرك إلى 90 درجة

    delay(2000);        // ينتظر ثانيتين

    myServo.write(0);   // يرجع إلى الصفر

    delay(300);         // منع تكرار الضغط بسرعة
  }
}