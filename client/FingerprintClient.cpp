#include "FingerprintClient.h"

FingerprintClient::FingerprintClient(HardwareSerial &uart) 
  : sensor(&uart) {}

void FingerprintClient::begin(){
    sensor.begin(57600);
}


bool FingerprintClient::getFingerprint(uint8_t buffer[]){
  
  
  Serial.println("Place your finger on the sensor...");

  while (sensor.getImage() != FINGERPRINT_OK);
  Serial.println("Fingerprint image captured.");
  
  if (sensor.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("Failed to convert image 1 to template.");
    return false;
  }

  while (sensor.getImage() != FINGERPRINT_OK);
  Serial.println("Fingerprint image captured.");
  
  if (sensor.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("Failed to convert image 2 to template.");
    return false;
  }

  if (sensor.createModel() != FINGERPRINT_OK) {
    Serial.println("Fingerprint model creation failed.");
    return false;
  }

  
  Serial.println("Pushing template from sensor buffer to UART");
  int result = sensor.getModel1();
  if (result != FINGERPRINT_OK) return false;

  if (sensor.get_template_buffer(FINGERPRINT_TEMPLATE_SIZE, buffer) == FINGERPRINT_OK) { 

    //Serial.println("Template data from sensor buffer");

    /*
    int j = 0;
    for (int i = 0; i < FINGERPRINT_TEMPLATE_SIZE; i++) {
        Serial.print("0x"); 
        Serial.print(buffer[i], HEX);
        Serial.print(",");
        j++;
        if (j == 128){
          Serial.println();
          j = 0;
        }
    }
    */
  }



  return true;
}













