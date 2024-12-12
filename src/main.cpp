#include <Arduino.h>
#include <painlessMesh.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_NeoPixel.h>
#include "MyServerCallbacks.h"
#include "esp_wifi.h"

//REDE MESH
#define MESH_PREFIX "REDE_PERIGOSA"
#define MESH_PASSWORD "naoentrapfv"
#define MESH_PORT 5555

//BLE
#define CHARACTERISTIC_UUID "69617e3a-d90b-4039-b7c5-f3350f070a05"
#define SERVICE_UUID "00c5808f-6f21-4e5c-9e19-f52ef38ab4ee"

//INTERVALOS DE TEMPO
#define BLE_TIMEOUT 5000
#define SWITCH_INTERVAL 10000
#define NOTIFICATION_INTERVAL 2000

//LED RGB
#define PIN 48 // ESP32-C3 built-in RGB led
#define NUMPIXELS 1
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4

//VARIÁVEIS AUXILIARES
bool bleActive = false;
bool bleConfigured = false;
bool deviceConnected = false;
String bleReceivedData = "";
String bleToSendData = "";
String meshData = "";
int ledColor = 0;
unsigned long lastSwitchTime = 0;
unsigned long bleStartTime = 0;
unsigned long notification_switch = 0;

painlessMesh mesh;
Scheduler myScheduler;

void sendMsg(std::string meshData) {
  uint32_t nodeId = mesh.getNodeId();
  String msg = String(meshData.c_str());
  mesh.sendBroadcast(msg);
}

void receivedMsg(uint32_t from, String &msg) {
  Serial.printf("Mensagem recebida de %u: %s\n", from, msg.c_str());
  bleToSendData = msg;
}

void setupMesh() {
  esp_wifi_stop();
  delay(100);
  esp_wifi_start();
  delay(100);
  //mesh.setDebugMsgTypes(ERROR | COMMUNICATION | STARTUP | CONNECTION);
  //mesh.setDebugMsgTypes(COMMUNICATION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &myScheduler, MESH_PORT, WIFI_AP_STA);
  mesh.onReceive(&receivedMsg);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("INICIANDO...");

  //INICIALIZAÇÃO DA MESH
  setupMesh();

  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  neopixelWrite(PIN,100,0,0);
}

void loop()
{
  if(bleActive) {
    if(!bleConfigured) {
      mesh.stop();
      
      BLEDevice::init("ESP32-PC");
      BLEServer *pServer = BLEDevice::createServer();
      BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
      BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
      );
      pServer->setCallbacks(new ServerCallbacks());
      pCharacteristic->setCallbacks(new CharacteristicCallbacks());
      pService->start();

      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->start();
      bleStartTime = millis();
      bleConfigured = true;
    } else if((millis() - bleStartTime) >= BLE_TIMEOUT && !deviceConnected) {
      if(ledColor != GREEN) {
        Serial.println("DESLIGANDO BLUETOOTH");
        neopixelWrite(PIN,0,100,0);
        ledColor = GREEN;
      }
      BLEDevice::getAdvertising()->stop();
      BLEDevice::deinit(false);
      bleActive = false;
      bleConfigured = false;
      setupMesh();
      lastSwitchTime = millis();
    }
  } else {
    //REDE MESH
    if (mesh.getNodeList().size() > 0 && !meshData.isEmpty()) {
      // Envio da mensagem recebida por BLE para todos os nós na rede mesh
      Serial.println("ENTROU NO ENVIO DE MSG");
      sendMsg(meshData.c_str());
      meshData.clear();
      bleActive = true;
      lastSwitchTime = millis();
    }
    mesh.update();
  }
  if(meshData.isEmpty() && (millis() - lastSwitchTime >= SWITCH_INTERVAL)) {
    lastSwitchTime = millis();
    bleActive = true;
    if(ledColor != BLUE) {
      Serial.println("LIGANDO BLUETOOTH");
      neopixelWrite(PIN,0,0,100);
      ledColor = BLUE;
    }
  } else if(!bleToSendData.isEmpty() && (millis() - notification_switch >= NOTIFICATION_INTERVAL)) {
    if(ledColor != YELLOW) {
      Serial.println("CONTEÚDO DISPONÍVEL");
      neopixelWrite(PIN,100,100,0);
      ledColor = YELLOW;
      notification_switch = millis();
    }
  }
}