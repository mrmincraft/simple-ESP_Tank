#include "MTcontroler.h"

MT_ctl::MT_ctl(){
    
}
MT_ctl::MT_ctl(uint8_t apin, uint8_t bpin, uint8_t enpin)
{
    MTa = apin;
    MTb = bpin;
    MTe = enpin;
}

void MT_ctl::init(int freq, uint8_t ch, uint8_t res){
    pinMode(MTa, OUTPUT);
    pinMode(MTb, OUTPUT);
    pinMode(MTe, OUTPUT);
    ledcSetup(ch, freq, res);
    ledcAttachPin(MTe, ch);
    MT_ch = ch;
    curentSpeed = 200;

}

void MT_ctl::test(){
    this -> set_speed(200);
    Serial.println("testing_MT");
    this-> forward();
    Serial.println("MT forward");
    delay(1000);
    this-> backward();
    Serial.println("MT backward");
    delay(1000);
    this-> standby();
    Serial.println("MT standby");
}

void MT_ctl::forward(u_int8_t speed){
    digitalWrite(MTa,HIGH);
    digitalWrite(MTb,LOW);
    ledcWrite(MT_ch,speed);
}
void MT_ctl::forward(){
    digitalWrite(MTa,HIGH);
    digitalWrite(MTb,LOW);
    ledcWrite(MT_ch,this-> curentSpeed);
}

void MT_ctl::backward(u_int8_t speed){
    digitalWrite(MTb,HIGH);
    digitalWrite(MTa,LOW);
    ledcWrite(MT_ch, this-> curentSpeed);
}
void MT_ctl::backward(){
    digitalWrite(MTb,HIGH);
    digitalWrite(MTa,LOW);
    ledcWrite(MT_ch,200);
}

void MT_ctl::standby(){
    digitalWrite(MTa,LOW);
    digitalWrite(MTb,LOW);
}

void MT_ctl::mbreak(){
    digitalWrite(MTa,HIGH);
    digitalWrite(MTb,HIGH);
}

void MT_ctl::set_speed(uint8_t dutycicle){
    ledcWrite(MT_ch,dutycicle);
}