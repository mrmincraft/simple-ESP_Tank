#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

//#define REVERSIBLE_ESC //coment if your ESC is configurated with only direction (non-reversible, only foward or foward/break)
#define OUTPUT_PITCH 33
#define OUTPUT_YAW 32
#define OUTPUT_ESC 19 //Signal output pin
#define PLANE_ESC_PWM_MIN 1000
#define PLANE_ESC_PWM_MAX 2000
#define PLANE_SERVO_MIN 0
#define PLANE_SERVO_MAX 180

u_int16_t DutyCycle = 0;    //Duty Cycle of the pwm signal generated
Servo _aileronPitch;
Servo _aileronYaw;
Servo _esc;

typedef struct struct_message {
  int x;
  int y;
  uint16_t z;
} struct_message;

struct_message myData;

void OnDataRecv(const uint8_t * mac, const u_int8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  if (map(myData.z, 1000, 2000, 3275, 6553) != DutyCycle) {
    DutyCycle = map(myData.z, 1000, 2000, 3275, 6553);  // Dutycycle to generate a pwm signal with that width in microseconds(us)
    _esc.write(DutyCycle);
  }
  int pitch = map(myData.x, -255, 255, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
  int yaw = map(myData.y, -255, 255, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
  _aileronPitch.write(pitch);
  _aileronYaw.write(yaw);
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
   

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print( WiFi.macAddress());
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

    
  ESP32PWM::allocateTimer(1);
  _aileronPitch.attach(OUTPUT_PITCH);
  ESP32PWM::allocateTimer(2);
  _aileronYaw.attach(OUTPUT_YAW);
  ESP32PWM::allocateTimer(0);
  _esc.setPeriodHertz(50);
  _esc.attach(OUTPUT_ESC);
  _esc.write(PLANE_ESC_PWM_MAX / 2);
}//end setup()

void loop() {
    delay(20); //delay fo An accel ramp, witch is very usefull for safe controling of your motor
}//end loop()