#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <DHT.h>

#define DHT_SENSOR_PIN  5 // ESP8266 pin GIOP5 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const char* ssid = "ssid";
const char* password = "password";
const char* serverUrl = "https://localhost:44301";

const char* postEndpointOutdoor = "/UOJ/OutdoorParameters";

const char* getEndpointOutdoor = "/UOJ/OutdoorParameters/1";

int OutdoorTemperature;
int OutdoorHumidity;
int DeviceWifiStrength;

void setup() {
  Serial.begin(115200);
  Wifi();
}

void loop() {
  WifiStrengthRead();
  OutdoorDHTRead();
  PostOutdoorParameters();
  Serial.println("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

void PostOutdoorParameters(){
  HTTPClient http;  
  String postURL = String(serverUrl) + postEndpointOutdoor;
  http.begin(postURL);
  http.addHeader("Content-Type", "application/json");
  
  //String jsonPayload = "{\"DeviceId\": 1,\"Temperature\": "+String(tempC)+ " ,\"Ph\": 2.0,\"Tds\": 3.0,\"WaterLevel\": "+ String(WaterLevelcm) +",\"BatteryVoltage\": 3.0,\"WifiStrength\": "+String(WiFi.RSSI())+ ",\"DeviceDateTime\": \"2023-07-06-14.55.23\",\"AirValve\": \"ON\",\"WaterPump\": \"ON\",\"DrainValve\": \"ON\"}";
  String jsonPayload = "{\"deviceId\":1,\"outdoorTemperature\":\"" + String(OutdoorTemperature) +"\",\"outdoorHumidity\":\"" + String(OutdoorHumidity) + "\",\"wifiStrength\":\""+String(DeviceWifiStrength)+"\",\"deviceDateTime\":\"2023-08-21 22:23:00\"}";

  int postResponseCode = http.POST(jsonPayload);

  if (postResponseCode > 0) {
    String postResponseBody = http.getString();
    Serial.println("POST response: " + postResponseBody);    
  } else {
    Serial.println("POST request failed");
  }
  http.end();
}

 void Wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void WifiStrengthRead(){
  DeviceWifiStrength = WiFi.RSSI();
  Serial.println("DeviceWifiStrength :" +String(DeviceWifiStrength));
}

void OutdoorDHTRead(){
    OutdoorHumidity  = dht_sensor.readHumidity();
    OutdoorHumidity = 90;  
    Serial.println("Outdoor Humidity: " + String(OutdoorHumidity));
    
    OutdoorTemperature = dht_sensor.readTemperature();
    OutdoorTemperature = 10;    
    Serial.println("Outdoor Temperature: " + String(OutdoorTemperature));
}

