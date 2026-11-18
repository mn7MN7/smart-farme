#include <Servo.h>

Servo myServo;

// تحديد المنافذ
const int rainSensorPin = A0; // منفذ المستشعر التناظري
const int servoPin = 9;        // منفذ محرك السرفو

// القيمة الحدية لكشف المطر (كلما قل الرقم زادت كمية الماء)
// يمكنك تعديل هذه القيمة حسب حساسية المستشعر المطلوب
int rainThreshold = 500; 

void setup() {
  Serial.begin(9600);          // تشغيل الشاشة الترتيبية لمراقبة القراءات
  myServo.attach(servoPin);    // ربط السرفو بالمنفذ
  myServo.write(0);            // الوضع الافتراضي: المظلة/النافذة مفتوحة (الزاوية 0)
  delay(500);
}

void loop() {
  int sensorValue = analogRead(rainSensorPin); // قراءة قيمة المستشعر

  // طباعة القراءة على الـ Serial Monitor لمتابعتها
  Serial.print("Rain Sensor Value: ");
  Serial.println(sensorValue);

  // إذا كانت القراءة أقل من الحد المطلوب (يعني وجود ماء/مطر)
  if (sensorValue < rainThreshold) {
    Serial.println("Status: Rain detected! Closing...");
    myServo.write(90); // تحريك السرفو للزاوية 90 (إغلاق)
  } else {
    Serial.println("Status: Dry. Opening...");
    myServo.write(0);  // إرجاع السرفو للزاوية 0 (فتح)
  }

  delay(500); // تأخير بسيط بين القراءات
}