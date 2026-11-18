#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// تحديد دبابيس التوصيل
#define DHTPIN 2          // دبوس بيانات مستشعر DHT11
#define DHTTYPE DHT11     // نوع المستشعر

#define RELAY_PIN 7       // دبوس التحكم بالمرحل (الفان)
#define LED_PIN 8         // دبوس التحكم بالـ LED (التدفئة)

// إعداد شاشة LCD بحجم 16x2 مع عنوان I2C الافتراضي 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // إعداد دبابيس المخرجات
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // إطفاء المخرجات في البداية
  // ملاحظة: معظم وحدات الريليه تعمل بمنطق معكوس (LOW = تشغيل، HIGH = إيقاف)
  digitalWrite(RELAY_PIN, HIGH); 
  digitalWrite(LED_PIN, LOW);

  // تشغيل الشاشة والمستشعر
  lcd.init();
  lcd.backlight();
  dht.begin();

  // رسالة ترحيبية قصيرة
  lcd.setCursor(0, 0);
  lcd.print(" Temp & Climate ");
  lcd.setCursor(0, 1);
  lcd.print(" System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {
  // قراءة درجة الحرارة
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // التأكد من صحة القراءة من المستشعر
  if (isnan(temp) || isnan(humidity)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    delay(1000);
    return;
  }

  // عرض القراءات على شاشة LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print((char)223); // رمز درجة الحرارة C°
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print((int)humidity);
  lcd.print("%   ");

  // --- منطق التحكم بالنظام ---

  // إذا كانت درجة الحرارة أعلى من 25: يشتغل الفان وينطفئ الـ LED
  if (temp > 25.0) {
    digitalWrite(RELAY_PIN, LOW);  // تشغيل المرحل/الفان
    digitalWrite(LED_PIN, LOW);    // إيقاف الـ LED
  } 
  // إذا كانت درجة الحرارة أقل من 23: يشتغل الـ LED وينطفئ الفان
  else if (temp < 23.0) {
    digitalWrite(RELAY_PIN, HIGH); // إيقاف المرحل/الفان
    digitalWrite(LED_PIN, HIGH);   // تشغيل الـ LED
  } 
  // في المجال بين 23 و 25: ينطفئ الاثنان (منطقة استقرار)
  else {
    digitalWrite(RELAY_PIN, HIGH); // إيقاف المرحل
    digitalWrite(LED_PIN, LOW);    // إيقاف الـ LED
  }

  delay(1000); // تحديث كل ثانية
}