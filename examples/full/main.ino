#include <ArduinoBleOTA.h>
#include <BleOtaSecurityOnConnect.h>

#define DEVICE_NAME "ArduinoBleOTA"

#ifdef ARDUINO_ARCH_ESP32
  #define HW_NAME "Example ESP32"
#elif ARDUINO_ARCH_ATMELSAM
  #define HW_NAME "Example ATMELSAM"
#else
  #define HW_NAME "Example HW"
#endif

#define HW_VER {1, 0, 0}
#define SW_NAME "Example SW"
#define SW_VER {1, 0, 0}

#ifdef USE_NIM_BLE_ARDUINO_LIB
BleOtaSecurityOnConnect security;
#endif

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
bool flag01 = 0;
int i = 0;
int reason ; 
int ledState = LOW;  // ledState used to set the LED
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis;

RTC_DATA_ATTR int bootCount = 0;
const int LED_PIN = LED_BUILTIN;  // the number of the LED pin
const long interval = 50000;  // interval at which to blink (milliseconds)


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); 
    reason = 1; 
     break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void setup() {

  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor
  pinMode(LED_PIN, OUTPUT);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
  //flag01 = 0;

  ArduinoBleOTA.begin(DEVICE_NAME, InternalStorage, HW_NAME, HW_VER, SW_NAME, SW_VER);

#ifdef USE_NIM_BLE_ARDUINO_LIB
  ArduinoBleOTA.setSecurityCallbacks(security);
  security.begin();
#endif
}

void loop() 
{
  Serial.print("flag: ");
  Serial.println(flag01);
  Serial.println("....   ");


if   (flag01 == 0) {
  
  unsigned long currentMillis = millis();
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  // if (reason == 1) {} 
  while (millis() - currentMillis < interval) {
       ledState = HIGH;
        digitalWrite(LED_PIN, ledState);
#ifdef USE_ARDUINO_BLE_LIB
  BLE.poll();
#endif
  ArduinoBleOTA.pull();

} 
  ledState = LOW;
  digitalWrite(LED_PIN, ledState);
  flag01 = 1;  // -> save to rtc memory???
  previousMillis = currentMillis;

  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
  }}
