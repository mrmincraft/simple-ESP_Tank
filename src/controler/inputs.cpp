#include "inputs.h"
#include "com.h"


//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
ESP32Encoder encoder;
int x = 0 ;
int old_x = 0;
int y = 0 ;
int old_y = 0;

void input_setup()
{
    pinMode(POT_1, INPUT);
    pinMode(POT_2, INPUT);
    encoder.attachHalfQuad(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
    encoder.setCount(0);
    //we must initialize rotary encoder
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    //set boundaries and if values should cycle or not
    //in this example we will set possible values between 0 and 1000;
    bool circleValues = true;
    rotaryEncoder.setBoundaries(0, 255, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

    /*Rotary acceleration introduced 25.2.2021.
   * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
   * without accelerateion you need long time to get to that number
   * Using acceleration, faster you turn, faster will the value raise.
   * For fine tuning slow down.
   */
    //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
    rotaryEncoder.setAcceleration(160); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
}

void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0; // Soft debouncing
    if (millis() - lastTimePressed < 500)
    {
            return;
    }
    lastTimePressed = millis();
    Serial.print("button pressed ");
    Serial.print(millis());
    Serial.println(" milliseconds after restart");
}

void rotary_loop()
{
    extern struct_message myData;
    //dont print anything unless value changed
    if (rotaryEncoder.encoderChanged())
    {
            Serial.print("Value: ");
            Serial.println(rotaryEncoder.readEncoder());
            serial_loop();
            
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
        delay(10);
    }
    joystickToMotor(POT_1,POT_2,x,y);
    if (x != old_x || y != old_y)
    {
        old_x = x ;
        old_y = y ;
        myData.x = x;
        myData.y = y;
        Serial.printf("left: %d right: %d \n",x,y);
        serial_loop();
    }
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();

}

void joystickToMotor(int xInput, int yInput, int &leftMotor, int &rightMotor) {
    // Map joystick input (0–4095) to -255 to 255
    int x = analogRead(xInput);
    int y = analogRead(yInput);

    // Map joystick values from 0–1023 to -255 to +255
    int mappedX = map(x, 0, 4095, -255, 255);
    int mappedY = map(y, 0, 4095, -255, 255);

      // Apply deadzone
    if (abs(mappedX) < DEADZONE) mappedX = 0;
    if (abs(mappedY) < DEADZONE) mappedY = 0;

    // Differential drive mixing
    int leftMotorSpeed = mappedY + mappedX;
    int rightMotorSpeed = mappedY - mappedX;

    // Clamp values to -255 to 255
    leftMotor  = constrain((int)leftMotorSpeed, -255, 255);
    rightMotor = constrain((int)rightMotorSpeed, -255, 255);
}
