//Health Monitoring System
#define BLYNK_TEMPLATE_ID "TMPL3o1YfsfFK"
#define BLYNK_TEMPLATE_NAME "Health Monitoring using ESP32"
#define BLYNK_AUTH_TOKEN "qXC9VLjswJ7LhLgeYauOEvS_Okaqevhe"

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define REPORTING_PERIOD_MS 1000
#define DS18B20 5


char auth[] = "qXC9VLjswJ7LhLgeYauOEvS_Okaqevhe";
char ssid[] = "realme narzo 30";
char pass[] = "sakuracluster";
PulseOximeter pox;

uint8_t DHTPin = 18;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
float bodytemperature;
float BPM, SpO2;
uint32_t tsLastReport = 0;

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

void onBeatDetected() {
  Serial.println("Beat Detected!");
}

void setup() {
  Serial.begin(115200);
  pinMode(19, OUTPUT);
  pinMode(DHTPin, INPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();
  Blynk.run();
  Temperature = dht.readTemperature(); 
  Humidity = dht.readHumidity();       
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  bodytemperature = sensors.getTempCByIndex(0);

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");
    Serial.print("Room Temperature: ");
    Serial.print(Temperature);
    Serial.println("°C");
    Serial.print("Room Humidity: ");
    Serial.print(Humidity);
    Serial.println("%");
    Serial.print("Body Temperature: ");
    Serial.print(bodytemperature);
    Serial.println("°C");
    Blynk.virtualWrite(V3, Temperature);
    Blynk.virtualWrite(V4, Humidity);
    Blynk.virtualWrite(V5, BPM);
    Blynk.virtualWrite(V6, SpO2);
    Blynk.virtualWrite(V7, bodytemperature);

    tsLastReport = millis();
  }
}