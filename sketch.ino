#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// I2C LCD Ekran Ayarları (Adres genellikle 0x27 veya 0x3F olur)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT22 Ayarları
#define DHTPIN 5     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

// Buton Pinleri
const int btnMesafe = 2;
const int btnSicaklik = 3;
const int btnIsik = 4;

// Ultrasonik Sensör Pinleri (HC-SR04)
const int trigPin = 7;
const int echoPin = 8;

// Işık Sensörü (LDR Modülü AO pini)
const int ldrPin = A0;

void setup() {
  Serial.begin(9600);
  
  // LCD Ekranı Başlatma
  lcd.init();                      
  lcd.backlight();
  
  // DHT Sensörünü Başlatma
  dht.begin();

  // Butonları Dahili Pull-Up Direnci ile Ayarlama
  pinMode(btnMesafe, INPUT_PULLUP);
  pinMode(btnSicaklik, INPUT_PULLUP);
  pinMode(btnIsik, INPUT_PULLUP);

  // Ultrasonik Pinleri
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Açılış Mesajı
  lcd.setCursor(0, 0);
  lcd.print("Proje Hazir!");
  lcd.setCursor(0, 1);
  lcd.print("Butona Basin...");
  
  Serial.println("Sistem baslatildi. Butonlara basabilirsiniz.");
}

void loop() {
  // 1. BUTON: Mesafe Ölçümü (Buton 2'ye bağlı)
  if (digitalRead(btnMesafe) == LOW) {
    long duration;
    float distance;
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    
    // Seri Monitöre Yazdır
    Serial.print("Mesafe: ");
    Serial.print(distance);
    Serial.println(" cm");

    // LCD Ekrana Yazdır
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mesafe Olarak:");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print(" cm");
    
    delay(400); // Debounce / Bekleme
  }

  // 2. BUTON: Sıcaklık Ölçümü (Buton 3'e bağlı)
  if (digitalRead(btnSicaklik) == LOW) {
    float temp = dht.readTemperature();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sicaklik Olcumu:");
    
    lcd.setCursor(0, 1);
    if (isnan(temp)) {
      Serial.println("Sicaklik okunamadi!");
      lcd.print("Hata!");
    } else {
      Serial.print("Sicaklik: ");
      Serial.print(temp);
      Serial.println(" C");
      
      lcd.print(temp);
      lcd.print(" C");
    }
    
    delay(400);
  }

  // 3. BUTON: Işık / Karanlık Durumu (Buton 4'e bağlı)
  if (digitalRead(btnIsik) == LOW) {
    int ldrValue = analogRead(ldrPin);
    
    Serial.print("LDR Degeri: ");
    Serial.print(ldrValue);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Isik Durumu:");
    
    lcd.setCursor(0, 1);
    // Modüle göre eşik değeri değişebilir (Genelde 500 civarı orta noktadır)
    if (ldrValue < 500) {
      Serial.println(" -> Aydinlik");
      lcd.print("Aydinlik (");
      lcd.print(ldrValue);
      lcd.print(")");
    } else {
      Serial.println(" -> Karanlik");
      lcd.print("Karanlik (");
      lcd.print(ldrValue);
      lcd.print(")");
    }
    
    delay(400);
  }
}