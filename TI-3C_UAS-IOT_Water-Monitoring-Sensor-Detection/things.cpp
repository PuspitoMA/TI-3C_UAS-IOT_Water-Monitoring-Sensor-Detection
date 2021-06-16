#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //atau 0x3F

#define triggerPin D3
#define echoPin D4
#define pinLed D0
int sensorPin = A0;    // input for LDR and rain sensor
int enable2 = 13;      // enable reading Rain sensor
int sensorValue2 = 0;  // variable to store the value coming from sensor Rain sensor
#define ONE_WIRE_BUS D7

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensorSuhu(&oneWire);

long duration, jarak;

void setup() {
  pinMode(enable2, OUTPUT);
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pinLed, OUTPUT);
  lcd.init();
}

void loop() {
  lcd.init();
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.print("\nKedalaman air : ");
  Serial.print(jarak);
  Serial.print(" Cm");
  sensorSuhu.requestTemperatures(); //Command atau syntax untuk mendapatkan data pembacaan
  Serial.print("\nPembacaan suhu : ");
  Serial.print(sensorSuhu.getTempCByIndex(0)); //syntax untuk menampilkan suhu yang terbac
  Serial.print(" C");
  lcd.setCursor(0,0);
  lcd.println(jarak);
  lcd.print(" cm");
  lcd.println(" dan ");
  lcd.println(sensorSuhu.getTempCByIndex(0));
  lcd.print(" C");

  sensorValue2 = analogRead(sensorPin);
  if (sensorValue2 < 1)
  {
    Serial.print("\nRain not detected = ");
    Serial.print(sensorValue2);
    Serial.print(" mm");
    
    digitalWrite(enable2, LOW); 
  }
  else if(sensorValue2 >= 100 && sensorValue2 < 500)
  {
    Serial.print("\nRain is detected = Ringan - ");
    Serial.print(sensorValue2);
    Serial.print(" mm"); 
    digitalWrite(enable2, HIGH);
    digitalWrite(pinLed, LOW);
  }
  else if(sensorValue2 >= 500 && sensorValue2 < 1000)
  {
    Serial.print("\nRain is detected = Normal - ");
    Serial.print(sensorValue2);
    Serial.print(" mm"); 
    digitalWrite(enable2, HIGH);
    digitalWrite(pinLed, LOW);
  }
  else if(sensorValue2 >= 1000 && sensorValue2 < 2000)
  {
    Serial.print("\nRain is detected = Lebat - ");
    Serial.print(sensorValue2);
    Serial.print(" mm"); 
    digitalWrite(enable2, HIGH);
    digitalWrite(pinLed, HIGH);
  }
  else if(sensorValue2 >= 2000)
  {
    Serial.print("\nRain is detected = Sangat lebat - ");
    Serial.print(sensorValue2);
    Serial.print(" mm"); 
    digitalWrite(enable2, HIGH);
    digitalWrite(pinLed, HIGH);
  }

//Serial.print("Rain value:       ");
//Serial.println(sensorValue2);
  Serial.println();
  delay(100);
}