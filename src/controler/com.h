#pragma once
#ifndef com_h
#define com_h
#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
    int x;
    int y;
    uint16_t z;
} struct_message;

// Create a struct_message called myData
//extern struct_message myData;



void com_setup();
void serial_loop();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

#endif