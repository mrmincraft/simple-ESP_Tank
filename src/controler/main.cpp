#include <Arduino.h>
#include "inputs.h"
#include "com.h"

struct_message myData;

void setup()
{
  input_setup();
  com_setup();
}

void loop()
{
    //in loop call your custom function which will process rotary encoder values
    rotary_loop();
    delay(100); //or do whatever you need to do...
}