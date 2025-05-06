#include "com.h"
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x86, 0xF0, 0xF0};
  //08:3A:F2:86:F0:F0
esp_now_peer_info_t peerInfo;



void com_setup()
{
    WiFi.mode(WIFI_MODE_STA);
    Serial.begin(115200);
    Serial.println(WiFi.macAddress());
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }


    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;


    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    esp_now_register_send_cb(OnDataSent);
}


void serial_loop()
{
  extern struct_message myData;
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}



void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
}





