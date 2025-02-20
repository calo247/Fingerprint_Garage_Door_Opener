#include "BluetoothServer.h"

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;

//static uint8_t fingerprintBuffer[FINGERPRINT_TEMPLATE_SIZE] = {0};
static size_t receivedSize = 0;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Client connected!");

    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Client disconnected. Restarting advertisement...");
        pServer->startAdvertising();
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
private:
  BluetoothServer* server;

public:

  MyCallbacks(BluetoothServer* _server) : server(_server) {}
  
  void onWrite(BLECharacteristic *pCharacteristic) {


    if (server->receivedSize == 0) {
        Serial.println("Receiving fingerprint from client...");
        memset(server->fingerprintBuffer, 0, FINGERPRINT_TEMPLATE_SIZE); // Clear buffer at the start
    }

    
    const uint8_t* value = pCharacteristic->getData();
    size_t length = pCharacteristic->getLength();

    if (server->receivedSize + length <= FINGERPRINT_TEMPLATE_SIZE) {
        memcpy(&server->fingerprintBuffer[server->receivedSize], value, length);
        server->receivedSize += length;
        Serial.print(server->receivedSize);
        Serial.print(",");
    } else {
        Serial.println("Warning: Received more than expected. Ignoring extra data.");
        return;
    }

    if (server->receivedSize == FINGERPRINT_TEMPLATE_SIZE) {
        Serial.println();
        Serial.println("Full fingerprint template received!");
        Serial.println("Template data in fingerprintBuffer variable");
        
        int j = 0;
        for (int i = 0; i < FINGERPRINT_TEMPLATE_SIZE; i++) { 
          Serial.print(server->fingerprintBuffer[i], HEX);
          Serial.print(",");
          j++;
          if (j == 128){
            Serial.println();
            j = 0;
          }
        }
        
        server->receivedFingerprintFlag = true;
        Serial.println("Setting receivedFingerprint to true");
        server->receivedSize = 0;
      }
      
  }
};

BluetoothServer::BluetoothServer() : receivedSize(0) {
  memset(fingerprintBuffer, 0, FINGERPRINT_TEMPLATE_SIZE);
}

uint8_t* BluetoothServer::getFingerprintBuffer() {
  return fingerprintBuffer;
}

size_t BluetoothServer::getReceivedSize() {
  return receivedSize;
}

bool BluetoothServer::getReceivedFingerprintFlag(){
  return receivedFingerprintFlag;
}

void BluetoothServer::setReceivedFingerprintFlag(bool value){
  this->receivedFingerprintFlag = value;
}

void BluetoothServer::begin() {
    Serial.println("Starting BLE Server...");
    BLEDevice::init("ESP32C6_Server");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new MyCallbacks(this));

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();

    receivedFingerprintFlag = false;

    Serial.println("BLE Server is ready and advertising!");
}



