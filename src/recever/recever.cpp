#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>
#include "MTcontroler.h"


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  uint8_t x;
  uint8_t y;
  uint16_t z;
} struct_message;

// Create a struct_message called myData
struct_message myData;

const int MT_A1 = 33;
const int MT_A2 = 32;
const int MT_AE = 17;
const int MT_B1 = 25;
const int MT_B2 = 26;
const int MT_BE = 16;
const int SERVO_0 = 15;

MT_ctl Mt1 = MT_ctl(MT_A1,MT_A2,MT_AE);
MT_ctl Mt2 = MT_ctl(MT_B1,MT_B2,MT_BE);

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  int steer = map(myData.y, 254, 0, -255, 255);
  int speed = map(myData.x, 0, 254, -255, 255);
  int mt1_speed = abs(speed) - steer;
  int mt2_speed = abs(speed) + steer;
  mt1_speed = constrain(mt1_speed, 0, 255);
  mt2_speed = constrain(mt2_speed, 0, 255);

  if (speed > 0)
  {
    Mt1.forward(mt1_speed);
    Mt2.forward(mt2_speed);
  }
  else if (speed < 0)
  {
    Mt1.backward(mt1_speed);
    Mt2.backward(mt2_speed);
  }
  else if (speed = 0)
  {
    if (steer > 0)
    {
      Mt1.forward(200);
      Mt2.backward(200);
    }
    else if (steer < 0)
    {
      Mt1.backward(200);
      Mt2.forward(200);
    }
  } else
  {
    Mt1.standby();
    Mt2.standby();
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Mt1.init(3000,0,8);
  Mt2.init(3000,1,8);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print( WiFi.macAddress());
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  
}