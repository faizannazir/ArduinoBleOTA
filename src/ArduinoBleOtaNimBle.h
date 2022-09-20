#pragma once
#include "BleOtaStorage.h"
#include "BleOtaVersion.h"
#include <NimBLEDevice.h>

class BleOtaUploader;

class ArduinoBleOTAClass: public BLECharacteristicCallbacks
{
public:
    bool begin(const std::string& deviceName, OTAStorage& storage);
    bool begin(OTAStorage& storage);
    bool begin(const std::string& deviceName, OTAStorage& storage,
               const std::string& hwName, Version hwVersion,
               const std::string& swName, Version swVersion);
    bool begin(OTAStorage& storage,
               const std::string& hwName, Version hwVersion,
               const std::string& swName, Version swVersion);
    void pull();

private:
    friend BleOtaUploader;
    void begin(BLEService& service,
               const std::string& hwName, Version hwVersion,
               const std::string& swName, Version swVersion);
    void onWrite(BLECharacteristic* characteristic) override;
    void send(const uint8_t* data, size_t length);
    BLECharacteristic* txCharacteristic;
};

extern ArduinoBleOTAClass ArduinoBleOTA;