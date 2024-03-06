# 1 "C:\\Users\\HP840G~1\\AppData\\Local\\Temp\\tmpuy97sxm_"
#include <Arduino.h>
# 1 "E:/downloads/Esp32BleOta/ArduinoBleOTA/examples/full/main.ino"
#include <ArduinoBleOTA.h>
#include <BleOtaSecurityOnConnect.h>

#define DEVICE_NAME "ArduinoBleOTA"

#ifdef ARDUINO_ARCH_ESP32
  #define HW_NAME "Example Esp32"
#elif ARDUINO_ARCH_ATMELSAM
  #define HW_NAME "Example ATMELSAM"
#else
  #define HW_NAME "Example HW"
#endif

#define HW_VER {1, 0, 0}
#define SW_NAME "Example SW"
#define SW_VER {1, 1, 0}
void setup();
void loop();
#line 22 "E:/downloads/Esp32BleOta/ArduinoBleOTA/examples/full/main.ino"
void setup() {
  ArduinoBleOTA.begin(DEVICE_NAME, InternalStorage, HW_NAME, HW_VER, SW_NAME, SW_VER);





}

void loop() {
#ifdef USE_ARDUINO_BLE_LIB
  BLE.poll();
#endif
  ArduinoBleOTA.pull();
}