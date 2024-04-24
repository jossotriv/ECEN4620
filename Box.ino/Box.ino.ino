#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLEServer.h>
#include <Arduino.h>
#include <Wire.h>

#define LED_PIN     D3
#define NUM_LEDS    1

// NeoPixel setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

BLEClient* pClient;
bool doconnect = false;

// BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "XIAOESP32S3_BLE"

// Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;

BLEUUID serviceUUID("181A"); // Environmental Sensing
BLEUUID charUUID("2A59");    // Analog Output

char xVal[1024];

// Callback function that gets called when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      Serial.println("Device found. Connecting!");
    }
  }
};

void setup() {
  Serial.begin(115200);
  
  // NeoPixel initialization
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Client Setup
  Serial.println("Starting BLE client...");
  BLEDevice::init("XIAOESP32S3_Client");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  pClient = BLEDevice::createClient();
  pClient->connect(*pServerAddress);
  Serial.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return;
  }

  BLERemoteCharacteristic* pCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return;
  }
  Serial.println(" - Found light value characteristics");
  
  pCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.println("Notify received");
    Serial.print("Value: ");
    Serial.println(*pData);
    snprintf(xVal, sizeof(xVal), "%d", *pData);
  });

  doconnect = true;
  Serial.println("Receiving...");
}

void loop() {
  int signal = atoi(xVal);
  
  if (doconnect) {
    if (signal == 0) {
      Serial.println("BLUE LED turning on:");
      strip.setPixelColor(0, strip.Color(0, 0, 255)); // BLUE
    } else if (signal == 1) {
      Serial.println("RED LED turning on:");
      strip.setPixelColor(0, strip.Color(255, 0, 0)); // RED
    }
    strip.show(); // Update the strip to set the color

    if (signal == 1) {
      Serial.println("Signal Locked");
      delay(4000);
    } else {
      delay(1000);
    }
  } else {
    Serial.println("Not Connected");
  }
}
