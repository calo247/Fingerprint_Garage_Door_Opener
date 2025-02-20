#include "FingerprintServer.h"


FingerprintServer::FingerprintServer(HardwareSerial &uart) 
  : sensor(&uart) {}

void FingerprintServer::begin() {
    sensor.begin(57600);
}

bool FingerprintServer::enrollFingerprint(int id) {

  uint8_t result;

  Serial.print("Enrolling fingerprint ID: ");
  Serial.println(id);
  Serial.println("Place your finger on the sensor...");

  // Wait for the first fingerprint scan
  while (sensor.getImage() != FINGERPRINT_OK);
  Serial.println("Fingerprint image captured.");
  
  if (sensor.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("Failed to convert first scan to template.");
    return false;
  }
  
  Serial.println("Remove your finger and place it again...");
  delay(2000);

  // Wait for the second fingerprint scan
  while (sensor.getImage() != FINGERPRINT_OK);
  Serial.println("Fingerprint image captured again.");

  if (sensor.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("Failed to convert second scan to template.");
    return false;
  }
  
  // Create a fingerprint model by merging the two scans
  result = sensor.createModel();
  if (result != FINGERPRINT_OK) {
    Serial.print("Fingerprint model creation failed: ");
    Serial.println(result, HEX);
    return false;
  } else {
    Serial.println("Fingerprint model generated");
  }

  // Store the fingerprint model in the sensor memory
  if (sensor.storeModel(id) == FINGERPRINT_OK) {
    Serial.println("Fingerprint successfully stored!");
    delay(2000);
    return true;
  } else {
    Serial.println("Failed to store fingerprint.");
    return false;
  }
}


void FingerprintServer::writeReceivedFingerprint(uint8_t buffer[]){
  uint8_t result = sensor.downloadModel2();
  if(result == FINGERPRINT_OK){
    Serial.println("Prepping sensor buffer 2 for write");
  } else {
    Serial.print("Error prepping buffer 2 to write: ");
    Serial.println(result);
  }
  result = sensor.writeTemplateToSensorBuffer(FINGERPRINT_TEMPLATE_SIZE, buffer);
  if(result == FINGERPRINT_OK){
    Serial.println("Writing received fingerprint to buffer 2");
  } else {
    Serial.print("Error writing to buffer 2: ");
    Serial.println(result);
  }
}

void FingerprintServer::clearBuffers(){

  uint8_t* AABuffer = (uint8_t*)malloc(2048);
  uint8_t* BBBuffer = (uint8_t*)malloc(2048);
  memset(AABuffer, 0xAA, 2048);
  memset(BBBuffer, 0xBB, 2048);

  //clear buffer 1
  uint8_t result = sensor.downloadModel1();
  if(result == FINGERPRINT_OK){
    Serial.println("Prepping sensor buffer 1 for write");
  } else {
    Serial.print("Error prepping buffer 1 to write: ");
    Serial.println(result);
  }
  result = sensor.writeTemplateToSensorBuffer(2048, AABuffer);
  if(result == FINGERPRINT_OK){
    Serial.println("Writing AA to buffer 1");
  } else {
    Serial.print("Error writing to buffer 1: ");
    Serial.println(result);
  }

  //clear buffer 2
  result = sensor.downloadModel2();
  if(result == FINGERPRINT_OK){
    Serial.println("Prepping sensor buffer 2 for write");
  } else {
    Serial.print("Error prepping buffer 2 to write: ");
    Serial.println(result);
  }
  result = sensor.writeTemplateToSensorBuffer(2048, BBBuffer);
  if(result == FINGERPRINT_OK){
    Serial.println("Writing BB to buffer 2");
  } else {
    Serial.print("Error writing to buffer 2: ");
    Serial.println(result);
  }
}

void FingerprintServer::printBuffersToSerial(){

  uint8_t* templateBuffer1 = (uint8_t*)malloc(2048);
  uint8_t* templateBuffer2 = (uint8_t*)malloc(2048);

  uint8_t result = sensor.getModel1();
  if(result == FINGERPRINT_OK){
    Serial.println("Pushing sensor buffer 1 to UART");
  } else {
    Serial.print("Error pushing sensor buffer 1 to UART: ");
    Serial.println(result);
  }
  result = sensor.get_template_buffer(FINGERPRINT_TEMPLATE_SIZE, templateBuffer1);
  if(result == FINGERPRINT_OK){
    Serial.println("Getting sensor buffer 1 from UART");
  } else {
    Serial.print("Error getting sensor buffer 1 from UART: ");
    Serial.println(result);
  }
  Serial.println("Data in sensor buffer 1:");
  int j = 0;
  for (int i = 0; i < FINGERPRINT_TEMPLATE_SIZE; i++) {
      Serial.print("0x"); 
      Serial.print(templateBuffer1[i], HEX);
      Serial.print(",");
      j++;
      if (j == 128){
        Serial.println();
        j = 0;
      }
  }
  result = sensor.getModel2();
  if(result == FINGERPRINT_OK){
    Serial.println("Pushing sensor buffer 2 to UART");
  } else {
    Serial.print("Error pushing sensor buffer 2 to UART: ");
    Serial.println(result);
  }
  result = sensor.get_template_buffer(FINGERPRINT_TEMPLATE_SIZE, templateBuffer2);
  if(result == FINGERPRINT_OK){
    Serial.println("Getting sensor buffer 2 from UART");
  } else {
    Serial.print("Error getting sensor buffer 2 from UART: ");
    Serial.println(result);
  }
  Serial.println("Data in sensor buffer 2:");
  j = 0;
  for (int i = 0; i < FINGERPRINT_TEMPLATE_SIZE; i++) {
      Serial.print("0x"); 
      Serial.print(templateBuffer2[i], HEX);
      Serial.print(",");
      j++;
      if (j == 128){
        Serial.println();
        j = 0;
      }
  }
}










