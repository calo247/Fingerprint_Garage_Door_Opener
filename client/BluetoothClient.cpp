#include "BluetoothClient.h"

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

static BLEAdvertisedDevice *myDevice;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic = nullptr;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
            Serial.println("Found BLE Server! Connecting...");
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            doConnect = true;
        }
    }
};

BluetoothClient::BluetoothClient() {}

void BluetoothClient::begin() {
    Serial.println("Starting BLE Client...");
    BLEDevice::init("ESP32C6_Client");

    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);

    if (doConnect) {
        BLEClient* pClient = BLEDevice::createClient();
        Serial.println("Connecting to BLE server...");
        pClient->connect(myDevice);
        BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);

        if (pRemoteService) {
            pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
            if (pRemoteCharacteristic) {
                connected = true;
                Serial.println("Connected to BLE Server!");
                Serial.print("MTU Size: ");
                Serial.println(BLEDevice::getMTU());

            } else {
                Serial.println("Failed to get remote characteristic.");
            }
        } else {
            Serial.println("Failed to connect to remote service.");
        }
    }
}



 


bool BluetoothClient::sendFingerprint(uint8_t *data, size_t length) {
    if (!connected) {
        Serial.println("Not connected to BLE server.");
        return false;
    }

    Serial.println("Sending fingerprint data over BLE...");

    uint16_t MTU = BLEDevice::getMTU() - 3;

    for (size_t i = 0; i < length; i += MTU) { 

      
        int chunkSize = min((int)MTU, (int)(length - i));  
      
        for (int j = 0; j < chunkSize; j++) {
            Serial.print(data[i + j], HEX);
            Serial.print(",");
        }
        Serial.println();
      
        pRemoteCharacteristic->writeValue(&data[i], chunkSize);
        delay(10);  // Small delay for BLE stability
    }

    Serial.println("Fingerprint data sent!");
    return true;
}
