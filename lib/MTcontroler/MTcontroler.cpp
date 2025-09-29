#include "MTcontroler.h"

MT_ctl::MT_ctl(){
    
}
MT_ctl::MT_ctl(uint8_t apin, uint8_t bpin, uint8_t enpin)
{
    this->MTa = apin;
    this->MTb = bpin;
    this->MTe = enpin;
}

void MT_ctl::init(int freq, uint8_t ch, uint8_t res){
    pinMode(this->MTa, OUTPUT);
    pinMode(this->MTb, OUTPUT);
    pinMode(this->MTe, OUTPUT);
    ledcSetup(ch, freq, res);
    ledcAttachPin(this->MTe, ch);
    this->MT_ch = ch;
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
    digitalWrite(this->MTa, HIGH);
    digitalWrite(this->MTb, LOW);
    ledcWrite(this->MT_ch, speed);
}
void MT_ctl::forward(){
    digitalWrite(this->MTa, HIGH);
    digitalWrite(this->MTb, LOW);
    ledcWrite(this->MT_ch, this->curentSpeed);
}

void MT_ctl::backward(u_int8_t speed){
    digitalWrite(this->MTb, HIGH);
    digitalWrite(this->MTa, LOW);
    ledcWrite(this->MT_ch, speed);
}
void MT_ctl::backward(){
    digitalWrite(this->MTb, HIGH);
    digitalWrite(this->MTa, LOW);
    ledcWrite(this->MT_ch, this->curentSpeed);
}

void MT_ctl::standby(){
    digitalWrite(this->MTa, LOW);
    digitalWrite(this->MTb, LOW);
}

void MT_ctl::mbreak(){
    digitalWrite(this->MTa, HIGH);
    digitalWrite(this->MTb, HIGH);
}

void MT_ctl::set_speed(uint8_t dutycicle){
    ledcWrite(this->MT_ch, dutycicle);
}