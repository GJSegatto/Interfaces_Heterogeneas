#ifndef MYSERVERCALLBACKS_H
#define MYSERVERCALLBACKS_H

#include <BLEServer.h>
#include <Arduino.h>

#define CHARACTERISTIC_UUID "69617e3a-d90b-4039-b7c5-f3350f070a05"
#define SERVICE_UUID "00c5808f-6f21-4e5c-9e19-f52ef38ab4ee"

extern bool deviceConnected;
extern String bleReceivedData;
extern String meshData;
extern String bleToSendData;

class ServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Dispositivo conectado.");
    deviceConnected = true;

    if(bleToSendData.length() > 0) {
      BLEService* pService = pServer->getServiceByUUID(BLEUUID(SERVICE_UUID));
      if(pService != nullptr) {
        BLECharacteristic* pCharacteristic =pService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
        pCharacteristic->setValue(bleToSendData.c_str());
        pCharacteristic->notify();
        bleToSendData.clear();
      }
    }
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Dispositivo desconectado.");
    deviceConnected = false;
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String bleReceivedData = pCharacteristic->getValue().c_str();
    if (bleReceivedData.length() > 0) {
      Serial.println("RECEBIDO:");
      Serial.println(bleReceivedData);
      meshData = bleReceivedData;
      bleReceivedData.clear();
    }
  }
};

#endif