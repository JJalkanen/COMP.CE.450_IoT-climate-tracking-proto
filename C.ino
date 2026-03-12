#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>

void setup() {

  if (!HTS.begin()) {
    while (1);
  }

  if (!BLE.begin()) {
    while (1);
  }

  BLE.scanForName("climate");
}

void loop() {

  BLEDevice peripheral = BLE.available();

  if (peripheral) {

    BLE.stopScan();

    if (peripheral.connect()) {
    } 
    
    else {
      return;
    }

    if (!peripheral.discoverAttributes()) {
      peripheral.disconnect();
      return;
    }

    BLECharacteristic temperatureCharacteristic =
      peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");

    BLECharacteristic humidityCharacteristic =
      peripheral.characteristic("19B10002-E8F2-537E-4F6C-D104768A1214");

    if (!temperatureCharacteristic || !humidityCharacteristic) {
      peripheral.disconnect();
      return;
    }

    while (peripheral.connected()) {
      
      float temperature = HTS.readTemperature();
      float humidity = HTS.readHumidity();

      

      temperatureCharacteristic.writeValue((byte*)&temperature, sizeof(temperature));
      humidityCharacteristic.writeValue((byte*)&humidity, sizeof(humidity));

      delay(2000);
    }

    BLE.scanForName("climate");
  }
}
