#ifndef FINGERPRINT_CLIENT_H
#define FINGERPRINT_CLIENT_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_TEMPLATE_SIZE 2048

class FingerprintClient {
  
public:
  Adafruit_Fingerprint sensor;

  FingerprintClient(HardwareSerial &uart);

  void begin();
  bool getFingerprint(uint8_t buffer[]);
  


  
    
};

#endif
