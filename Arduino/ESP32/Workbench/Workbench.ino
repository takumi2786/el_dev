// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <DHT.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// #define BUZZER_PIN 4

#define DHTTYPE DHT11   // DHT 11
#define MinHumidity 35
#define MaxHumidity 60

#define LedPinTooHIGH 27
#define LedPinOK      26
#define LedPinTooLOW  25

// 音階ごとの周波数
#define C4 261.6
#define C#4 277.18
#define D4 293.665
#define D#4 311.127
#define E4 329.63
#define F4 349.228
#define F#4 369.994
#define G4 391.995
#define G#4 415.305
#define A4 440
#define A#4 466.164
#define B4 493.883
#define C5 523.251

DHT dht(DHTPIN, DHTTYPE);

void read(float& h, float& t) {
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  return;
}

// 計測値を表示
void disp(float& h, float t, float hic){
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.print(hic);
  Serial.print("°C ");
  Serial.println("");
}


// 適正な湿度かチェック
void check(float h){
  if (h <= MinHumidity){
    Serial.println("Air is too dry!!");
    digitalWrite(LedPinTooHIGH, HIGH);
    digitalWrite(LedPinOK, LOW);
    digitalWrite(LedPinTooLOW, LOW);
  }else if (h >= MaxHumidity){
    Serial.println("Air is too Moisturized!!");
    digitalWrite(LedPinTooHIGH, HIGH);
    digitalWrite(LedPinOK, LOW);
    digitalWrite(LedPinTooLOW, LOW);
  }else{
    digitalWrite(LedPinTooHIGH, LOW);
    digitalWrite(LedPinOK, HIGH);
    digitalWrite(LedPinTooLOW, LOW);
  }

}


void setup() {
  Serial.begin(9600);

  // 表示用LED
  pinMode(LedPinTooHIGH, OUTPUT); 
  pinMode(LedPinOK, OUTPUT); 
  pinMode(LedPinTooLOW, OUTPUT); 
  dht.begin();
}

void loop() {
  
  // Wait a few seconds between measurements.
  delay(3000);

  float h,t;
  read(h,t);

  // 熱中症に関する指標らしい．
  // 要は体感温度
  float hic = dht.computeHeatIndex(t, h, false);

  // 計測値を表示
  disp(h,t,hic);
  
  // 適正な湿度かチェック
  check(h);

  // ローカルネットワークにウェブページとして公開


  
}


// スピーカーの操作 あとで調べる．
// ledcSetup(1,12000, 8);
// ledcAttachPin(BUZZER_PIN,1);
// ledcWriteTone(1,C4);
// delay(200);
// ledcWriteTone(1,B4);
// delay(200);
// ledcDetachPin(BUZZER_PIN);