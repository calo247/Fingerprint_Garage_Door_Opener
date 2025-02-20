#ifndef BLUETOOTH_CLIENT_H
#define BLUETOOTH_CLIENT_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>

class BluetoothClient {
public:
    BluetoothClient();
    void begin();
    bool sendFingerprint(uint8_t *data, size_t length);
};

#endif
