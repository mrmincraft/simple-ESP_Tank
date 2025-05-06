#include  <AiEsp32RotaryEncoder.h>
#include <ESP32Encoder.h>

#define ROTARY_ENCODER_A_PIN 26
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 2
#define BUZZER_PIN 33
#define POT_1 32
#define POT_2 33

void input_setup();
void rotary_onButtonClick();
void rotary_loop();
void IRAM_ATTR readEncoderISR();
int prosesValues(int value, bool reverse);