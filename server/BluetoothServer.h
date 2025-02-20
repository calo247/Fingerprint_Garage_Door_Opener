#ifndef BLUETOOTH_SERVER_H
#define BLUETOOTH_SERVER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define FINGERPRINT_TEMPLATE_SIZE 2048  // Expected size of fingerprint template

class BluetoothServer {
public:
    BluetoothServer();
    void begin();
    uint8_t* getFingerprintBuffer();
    size_t getReceivedSize();
    bool getReceivedFingerprintFlag();
    void setReceivedFingerprintFlag(bool value);

    uint8_t fingerprintBuffer[FINGERPRINT_TEMPLATE_SIZE];  

private:
    
    size_t receivedSize;  
    bool receivedFingerprintFlag;

    BLEServer *pServer;
    BLECharacteristic *pCharacteristic;

    friend class MyCallbacks;  
    //friend class FingerprintServer;
};

#endif
