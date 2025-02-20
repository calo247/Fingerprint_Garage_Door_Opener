#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "FingerprintClient.h"
#include "BluetoothClient.h"

HardwareSerial uart(1);
FingerprintClient finger(uart);
BluetoothClient bluetooth;

uint8_t* rawBuffer1 = (uint8_t*)malloc(12800);
uint8_t rawBuffer[2224] = {0};
uint8_t templateBuffer[2048] = {0};

void setup() {
  Serial.begin(57600);
  uart.begin(57600, SERIAL_8N1, D7, D6);
  Serial.println("Initializing Fingerprint Sensor...");
  finger.begin();
  bluetooth.begin();

  if (finger.sensor.verifyPassword()) {

    Serial.println("Fingerprint sensor detected!");

  } else {

    Serial.println("Fingerprint sensor NOT detected. Check wiring!");
    while (1);
  }
}

void loop() {


  
  if (finger.getFingerprint(templateBuffer)) {
    Serial.println("Fingerprint model extracted! Sending via BLE...");
    bluetooth.sendFingerprint(templateBuffer, FINGERPRINT_TEMPLATE_SIZE);
  } else {
    Serial.println("Failed to extract fingerprint model.");
  }

  delay(3000);


}






