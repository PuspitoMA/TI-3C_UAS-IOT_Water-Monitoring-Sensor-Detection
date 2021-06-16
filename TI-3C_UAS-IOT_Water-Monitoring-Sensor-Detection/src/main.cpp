#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

const char *ssid = "Bora";//silakan disesuaikan sendiri
const char *password = "tatamics";//silakan disesuaikan sendiri

const char *mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);

int sensorPin = A0;    // input for LDR and rain sensor
int enable2 = 13;      // enable reading Rain sensor
int sensorValue2 = 0;  // variable to store the value coming
int triggerPin = D3;
int echoPin = D4;
#define ONE_WIRE_BUS D1

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensorSuhu(&oneWire);
long now = millis();
long lastMeasure = 0;
long duration;
int jarak;

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  }

void level(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.print(jarak);

  delay(500);

}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect("ESP8266Client");
  }

  now = millis();
  
  byte celsius = 0;


  sensorValue2 = analogRead(sensorPin);
  sensorSuhu.requestTemperatures();

    // Temperature in Celsius degrees
  celsius = sensorSuhu.getTempCByIndex(0);
  static char temperaturenow [15];
  static char rainnow [16];
  static char levelnow [17];


  dtostrf(celsius,7, 3, temperaturenow);  //// convert float to char
  dtostrf(sensorValue2,7, 3, rainnow);  //// convert float to char
  dtostrf(jarak,7, 3, levelnow);  //// convert float to char
  Serial.print("Suhu Air : ");
  Serial.print(temperaturenow);
  Serial.println(" Celcius ");
  Serial.print("Intensitas Hujan : ");
  Serial.print(rainnow);
  Serial.println(" mm/jam ");
  Serial.print("Kedalaman : ");
  level();
  Serial.println(" cm ");
  client.publish("water/temp", temperaturenow);    /// send char
  client.publish("water/rain", rainnow);
  client.publish("water/kedalaman", levelnow); 
  }