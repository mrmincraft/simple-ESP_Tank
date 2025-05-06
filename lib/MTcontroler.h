#pragma once
#ifndef MTcontroler
#define MTcontroler

#if defined(ARDUINO_ARCH_ESP32) || defined(ESP8266)
#include <functional>
#endif
#include <Arduino.h>


class MT_ctl {
    public:
    MT_ctl();
    MT_ctl(uint8_t apin, uint8_t bpin, uint8_t enpin);
    void test();
    void forward(uint8_t speed);
    void forward();
    void backward(uint8_t speed);
    void backward();
    void standby();
    void mbreak();
    void set_speed(uint8_t dutycicle);
    void init(int freq, uint8_t ch, uint8_t res);

    protected:
    uint8_t MTa;
    uint8_t MTb;
    uint8_t MTe;
    uint8_t MT_ch;
    uint8_t curentSpeed;
    
    
};
#endif