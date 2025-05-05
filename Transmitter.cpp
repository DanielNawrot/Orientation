#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SPI.h>

#define BNO055_SAMPLERATE_DELAY_MS (50)
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

imu::Vector<3> orientation;

typedef struct struct_message
{
  float time;
  float orientX;
  float orientY;
  float orientZ;
  float distUS;
} struct_message;

// MUST CHECK FOR RECIEVER ADDRESS
// Replace all NN with proper address
uint8_t broadCastAddress[] = {0xNN, 0xNN, 0xNN, 0xNN, 0xNN, 0xNN};
struct_message message;
esp_now_peer_info peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ERROR initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, broadCastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  Wire.begin();

  if (!bno.begin())
  {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }

  delay(1000);
  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  while (gyro != 3 && mag != 3)
  {
    bno.getCalibration(&system, &gyro, &accel, &mag);

    Serial.print("Mag Calibration: ");
    Serial.println(mag, DEC);
    Serial.print("Gyro Calibration: ");
    Serial.println(gyro, DEC);
  }
}

void loop()
{
  sensors_event_t event;
  bno.getEvent(&event);

  orientation = {event.orientation.x, event.orientation.y, event.orientation.z};
  Serial.print(millis() / (float)1000, DEC);

  Serial.print(",");
  Serial.print(orientation.x());
  Serial.print(",");
  Serial.print(orientation.y());
  Serial.print(",");
  Serial.println(orientation.z());

  message.time = millis() / 1000.0;
  message.orientX = orientation.x();
  message.orientY = orientation.y();
  message.orientZ = orientation.z();
  message.distUS = 0;

  esp_err_t result = esp_now_send(0, (uint8_t *)&message, sizeof(struct_message));
  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }

  delay(100);
}
