#include <ArduinoBleOTA.h>

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
bool flag01 = 0;
int i = 0;

RTC_DATA_ATTR int bootCount = 0;
// constants won't change. Used here to set a pin number:
const int ledPin = GPIO_NUM_2; // the number of the LED pin

// Variables will change:
int ledState = LOW; // ledState used to set the LED

// constants won't change:
const long interval = 300000; // 5 minutes interval at which to blink (milliseconds)

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
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

void setup()
{
  ArduinoBleOTA.begin("ArduinoBleOTA", InternalStorage);

  Serial.begin(115200);
  delay(1000); // Take some time to open up the Serial Monitor
  pinMode(ledPin, OUTPUT);

  // Increment boot number and print it every reboot
  //++bootCount;
  // Serial.println("Boot number: " + String(bootCount));

  // Print the wakeup reason for ESP32
  // print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1 .
  ext0 uses RTC_IO to wakeup thus requires RTC peripherals
  to be on while ext1 uses RTC Controller so doesnt need
  peripherals to be powered on.
  Note that using internal pullups/pulldowns also requires
  RTC peripherals to be turned on.
  */
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); // 1 = High, 0 = Low
  // flag01 = 0;
  Serial.println(digitalRead(GPIO_NUM_33));
  // If you were to use ext1, you would use it like
  // esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  // Go to sleep now
  //  Serial.println("Going to sleep now");
  //  esp_deep_sleep_start();
  // Serial.println("This will never be printed");
}

void loop()
{
  Serial.println("flag: ");
  Serial.println(flag01);
  Serial.println("....   ");

  if (flag01 == 0)
  {

    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    // Print the wakeup reason for ESP32
    print_wakeup_reason();
    unsigned long currentMillis = millis();
    unsigned long previousMillis = 0;
    while (currentMillis - previousMillis <= interval)
    {
#ifdef USE_ARDUINO_BLE_LIB
      BLE.poll();
#endif
      ArduinoBleOTA.pull();
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW)
      {
        ledState = HIGH;
      }
      else
      {
        ledState = LOW;
      }
      // Update the previousMillis variable
      previousMillis = currentMillis;
    }
    previousMillis = 0;
  }

  Serial.println("Going to sleep now");
  flag01 = 1;
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}