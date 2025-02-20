#ifndef FINGERPRINT_SERVER_H
#define FINGERPRINT_SERVER_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_TEMPLATE_SIZE 2048

class FingerprintServer {
public:
  Adafruit_Fingerprint sensor;

  FingerprintServer(HardwareSerial &uart);

  void begin();
  bool enrollFingerprint(int id);
  void writeReceivedFingerprint(uint8_t buffer[]);
  void clearBuffers();
  void printBuffersToSerial();




};

#endif
