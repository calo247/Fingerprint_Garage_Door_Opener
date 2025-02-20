#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "FingerprintServer.h"
#include "BluetoothServer.h"

HardwareSerial uart(1);
FingerprintServer finger(uart);
BluetoothServer bluetooth;

uint8_t result;

void setup() {
  Serial.begin(57600);
  uart.begin(57600, SERIAL_8N1, D7, D6);
  Serial.println("Initializing Fingerprint Sensor...");
  finger.begin();  

  if (finger.sensor.verifyPassword()) {
    Serial.println("Fingerprint sensor detected!");
    finger.sensor.emptyDatabase();
    finger.enrollFingerprint(8);
  } else {
    Serial.println("Fingerprint sensor NOT detected. Check wiring!");
    while (1);
  }
  bluetooth.begin();

}



void loop() {


  if (bluetooth.getReceivedFingerprintFlag()){

    finger.clearBuffers();
    //printBuffersToSerial(); //DEBUG: write buffers to serial
    finger.writeReceivedFingerprint(bluetooth.fingerprintBuffer);
    //printBuffersToSerial(); //DEBUG: write buffers to serial
    
    result = finger.sensor.loadModelBuffer1(8);
    if(result == FINGERPRINT_OK){
      Serial.println("Moving #8 to buffer 1");
    } else {
      Serial.print("Error moving #8 to buffer 1: ");
      Serial.println(result);
    } 
    
    //printBuffersToSerial();

    result = finger.sensor.matchTemplateBuffers();
    if(result == FINGERPRINT_OK){
      Serial.println("Fingerprint buffers match");
    } else {
      Serial.print("Error with fingerprint buffer match: ");
      Serial.println(result);
    } 



    bluetooth.setReceivedFingerprintFlag(false); 
  }

  delay(50);
  
}












