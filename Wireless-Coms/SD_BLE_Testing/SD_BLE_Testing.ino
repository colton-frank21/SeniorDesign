#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

//setup stuff for BLE Server/Service/Characteristic
BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;

//Pins For Outputs
#define LEDPIN 1


void setup() {
  Serial.begin(115200);

  BLEDevice::init("SDTesting");        //Creating BLE Device
  pServer = BLEDevice::createServer(); //Sets BLE Device as Server

  pService = pServer->createService(SERVICE_UUID); //Creating Service for server to Host.

  //Setting Up characteristic properties, currently allowed to read and write
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, 
                                                   BLECharacteristic::PROPERTY_READ | 
                                                   BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Testing Setup");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); //Allow Devices to see ESP32
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();

  //Pin setup
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  String value = pCharacteristic->getValue(); //Reading in value from BLE
  Serial.println(value);

  if(value == "1"){
    digitalWrite(LEDPIN, HIGH);
  }
  else if(value == "0"){
    digitalWrite(LEDPIN, LOW);
  }

  delay(100);
}