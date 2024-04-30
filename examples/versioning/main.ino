#include <ArduinoBleOTA.h>

#define DEVICE_NAME "Esp32 BLE OTA"
#define HW_NAME "Esp32 HW"
#define HW_VER {1, 0, 0}
#define SW_NAME "Esp32 SW"
#define SW_VER {1, 4, 0}
#define LED_PIN 2

void setup() {
  ArduinoBleOTA.begin(DEVICE_NAME, InternalStorage, HW_NAME, HW_VER, SW_NAME, SW_VER);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500); // Adjust delay for desired blink rate
  digitalWrite(LED_PIN, LOW);
  delay(500);

// #ifdef USE_ARDUINO_BLE_LIB
//   BLE.poll();
// #endif
  ArduinoBleOTA.pull();
}