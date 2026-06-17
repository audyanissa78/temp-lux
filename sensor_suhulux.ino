#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- KONFIGURASI SENSOR ---
const int oneWireBus = 4; // Pin Data DS18B20 ke GPIO 4
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Sensor Lux BH1750 (SDA = 21, SCL = 22)
BH1750 lightMeter;

// MASUKKAN SCRIPT_ID MILIKMU DI SINI
String GOOGLE_SCRIPT_ID = "MASUKKAN_SCRIPT_ID_MU_DISINI"; 

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Sensor Suhu & Cahaya
  sensors.begin();
  Wire.begin();
  if (lightMeter.begin()) {
    Serial.println("BH1750 (Lux) siap!");
  } else {
    Serial.println("Gagal inisialisasi BH1750");
  }
  
  // Menjalankan WiFiManager
  WiFiManager wm;
  Serial.println("Menghidupkan mode setup WiFi...");
  
  // Pancarkan WiFi bernama "Sensor-Lab-Setup" jika belum konek
  bool res = wm.autoConnect("Sensor-Lab-Setup"); 
  if(!res) {
    Serial.println("Gagal terhubung, restart...");
    ESP.restart();
  }
  
  Serial.println("Berhasil terhubung ke WiFi lokal!");
}

void loop() {
  // 1. Membaca Sensor Suhu & Cahaya
  sensors.requestTemperatures(); 
  float suhu = sensors.getTempCByIndex(0);
  float lux = lightMeter.readLightLevel();
  
  // Tampilkan di Serial Monitor laptop untuk pantauan lokal
  Serial.print("Suhu: "); Serial.print(suhu); Serial.print(" C | ");
  Serial.print("Cahaya: "); Serial.print(lux); Serial.println(" Lux");
  
  // 2. Mengirim ke Google Sheets jika WiFi Konek
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Mengirim data ke lab...");
    
    HTTPClient http;
    // URL dirakit HANYA dengan suhu dan lux
    String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?suhu=" + String(suhu) + "&lux=" + String(lux);
    
    http.begin(url); 
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    int httpCode = http.GET();
    if(httpCode > 0) {
      String payload = http.getString();
      Serial.println("Respon Google: " + payload);
    } else {
      Serial.println("Gagal HTTP: " + http.errorToString(httpCode));
    }
    http.end();
  }
  
  // Jeda 5 menit (300.000 milidetik) sebelum membaca & mengirim lagi
  delay(300000); 
}