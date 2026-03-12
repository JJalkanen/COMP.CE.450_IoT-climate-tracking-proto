#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>

float t1 = 0;
float h1 = 0;
float t2 = 0;
float h2 = 0;


float* calculateMean(float temp1, float temp2, float humi1, float humi2) {
  static float result[2];
  result[0] = (temp1 + temp2) / 2;
  result[1] = (humi1 + humi2) / 2;
  return result;
}

BLEService climateService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEFloatCharacteristic temperatureCharacteristic(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

BLEFloatCharacteristic humidityCharacteristic(
  "19B10002-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

void setup() {

  Serial.begin(9600);
  while (!Serial);

  if (!HTS.begin()) {
    while (1);
  }

  if (!BLE.begin()) {
    while (1);
  }

  BLE.setLocalName("climate");
  BLE.setAdvertisedService(climateService);

  climateService.addCharacteristic(temperatureCharacteristic);
  climateService.addCharacteristic(humidityCharacteristic);
  
  BLE.addService(climateService);

  temperatureCharacteristic.writeValue(0.0);
  humidityCharacteristic.writeValue(0.0);
  
  BLE.advertise();

}

void loop() {

  BLEDevice central = BLE.central();

  if (central) {

    while (central.connected()) {

      BLE.poll();
      
      float t2 = HTS.readTemperature();
      float h2 = HTS.readHumidity();

      if (temperatureCharacteristic.written()) {
        t1 = temperatureCharacteristic.value();
      }

      if (humidityCharacteristic.written()) {
        h1 = humidityCharacteristic.value();
      }

      float* means = calculateMean(t1, t2, h1, h2);

      float meanTemp = means[0];
      float meanHum = means[1];
      
      /*Serial.println("---------------------------------------");

      Serial.print("Arduino1 Temp: ");
      Serial.println(t1);

      Serial.print("Arduino2 Temp: ");
      Serial.println(t2);

      Serial.print("Arduino1 Hum: ");
      Serial.println(h1);

      Serial.print("Arduino2 Hum: ");
      Serial.println(h2);

      Serial.println("");
      Serial.print("Mean Temp: ");
      Serial.println(meanTemp);
      Serial.print("Mean Hum: ");
      Serial.println(meanHum);
      Serial.println("---------------------------------------");
      */
      Serial.print(t1);
      Serial.print(",");
      Serial.print(t2);
      Serial.print(",");
      Serial.print(h1);
      Serial.print(",");
      Serial.print(h2);
      Serial.print(",");
      Serial.print(meanTemp);
      Serial.print(",");
      Serial.println(meanHum);
      delay(2000);
    }
  }
}
