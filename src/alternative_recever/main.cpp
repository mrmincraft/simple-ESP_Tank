#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Pins
#define OUTPUT_PITCH 33
#define OUTPUT_YAW   32
#define OUTPUT_ESC   19

// Servo/ESC objects
Servo pitchServo;
Servo yawServo;
Servo esc;

// RX packet struct (must match transmitter)
typedef struct struct_message {
  int x;        // pitch input  -255..255
  int y;        // yaw input    -255..255
  uint16_t z;   // throttle     1000..2000
} struct_message;

struct_message myData;

// Debug variables
int pitchAngle = 0;
int yawAngle   = 0;
int escPulse   = 1500;

void printDebug() {
  Serial.print("RX: X=");
  Serial.print(myData.x);
  Serial.print("  Y=");
  Serial.print(myData.y);
  Serial.print("  Z=");
  Serial.print(myData.z);
  Serial.print("  |  ");

  Serial.print("Pitch=");
  Serial.print(pitchAngle);
  Serial.print("°  Yaw=");
  Serial.print(yawAngle);
  Serial.print("°  ESC=");
  Serial.print(escPulse);
  Serial.println(" µs");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // Convert inputs → outputs
  pitchAngle = map(myData.x, -255, 255, 0, 180);
  yawAngle   = map(myData.y, -255, 255, 0, 180);
  escPulse   = map(myData.z, 1000, 2000, 1000, 2000);

  // Apply to hardware
  pitchServo.write(pitchAngle);
  yawServo.write(yawAngle);
  esc.writeMicroseconds(escPulse);

  // Print debug info
  printDebug();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP-NOW SERVO + ESC DEBUG RECEIVER ===");
  Serial.println("REMOVE PROPELLER BEFORE POWERING ESC!");
  delay(2000);

  // WiFi to STA mode
  WiFi.mode(WIFI_STA);
  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  // Register callback
  esp_now_register_recv_cb(OnDataRecv);

  // Attach outputs
  pitchServo.attach(OUTPUT_PITCH, 1000, 2000);
  yawServo.attach(OUTPUT_YAW, 1000, 2000);
  esc.attach(OUTPUT_ESC, 1000, 2000);

  // Arm ESC safely
  Serial.println("Arming ESC at 1500 µs...");
  esc.writeMicroseconds(1500);
  delay(2000);
}

void loop() {
  // Nothing here — all logic is in the callback
  delay(20);  // keeps system stable
}