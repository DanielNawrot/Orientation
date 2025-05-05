#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SPI.h>

typedef struct struct_message
{
  float time;
  float orientX;
  float orientY;
  float orientZ;
  float distUS;
} struct_message;

struct_message myData;

float inTime;
float inOrientX;
float inOrientY;
float inOrientZ;
float inDistUS;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  inTime = myData.time;
  inOrientX = myData.orientX;
  inOrientY = myData.orientY;
  inOrientZ = myData.orientZ;
  inDistUS = myData.distUS;

  // Serial.print("Bytes Recieved: ");
  // Serial.println(len);
  // Serial.print("Orient : ");
  Serial.print(inTime);
  Serial.print(",");
  Serial.print(inOrientX);
  Serial.print(",");
  Serial.print(inOrientY);
  Serial.print(",");
  Serial.print(inOrientZ);
  Serial.print(",");
  Serial.println(inDistUS);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32 Started");
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  delay(250);
  Serial.println("Ready to recieve");
  Serial.println(WiFi.macAddress());
}

void loop()
{
  delay(1000);
}
