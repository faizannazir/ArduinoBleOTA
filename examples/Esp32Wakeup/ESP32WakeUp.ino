#include <ArduinoBleOTA.h>

#define DEVICE_NAME "ArduinoBleOTA"
#define HW_NAME "Esp32 HW"
#define HW_VER {1, 0, 0}
#define SW_NAME "Esp32 SW"
#define SW_VER {1, 1, 0}


#define WAKEUP_TIMER 30   // 30 seconds
#define WAKEUP_DURATION_TIMER 15  // 15 seconds
#define WAKEUP_DURATION_BUTTON 30 // 30 seconds
#define LED_PIN 2        // Built-in LED pin for TTGO T7 v1.5



RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by touchpad");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Take some time to open up the Serial Monitor

  // Configure GPIO 33 as input with internal pull-up resistor enabled
  pinMode(33, INPUT_PULLUP);

  // Configure built-in LED pin as output
  pinMode(LED_PIN, OUTPUT);


  // Increment boot count
  ++bootCount;
  Serial.println("Boot count: " + String(bootCount));

  print_wakeup_reason();
  // Determine wake-up duration based on the wake-up reason
  unsigned long wakeup_duration;
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    wakeup_duration = WAKEUP_DURATION_TIMER * 1000;
  } else if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    wakeup_duration = WAKEUP_DURATION_BUTTON * 1000;
  } else {
    // Default to 20 seconds if the wake-up reason is unknown
    wakeup_duration = 20000;
  }

  // Stay awake for the determined duration
  unsigned long wakeup_time = millis() + wakeup_duration;
  unsigned long led_toggle_time = millis();
  bool led_state = false;
  while (millis() < wakeup_time) {
    // Blink LED if wakeup was caused by timer
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
      if (millis() - led_toggle_time >= 500) {
        led_toggle_time = millis();
        led_state = !led_state;
        digitalWrite(LED_PIN, led_state);
      }
    }
    // Keep LED on if wakeup was caused by button press
    else if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
      delay(1000);
    ArduinoBleOTA.begin(DEVICE_NAME, InternalStorage, HW_NAME, HW_VER, SW_NAME, SW_VER);
      digitalWrite(LED_PIN, HIGH);
        #ifdef USE_ARDUINO_BLE_LIB
          BLE.poll();
        #endif
          ArduinoBleOTA.pull();
    }
    }

  // Setup timer for wakeup
  esp_sleep_enable_timer_wakeup(WAKEUP_TIMER * 1000000ULL);

  // Setup external wakeup on GPIO 33 (high)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

  // Go to deep sleep
  Serial.println("Going to deep sleep...");
  esp_deep_sleep_start();
}

void loop() {

}