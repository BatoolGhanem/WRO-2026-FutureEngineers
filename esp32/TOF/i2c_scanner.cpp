#include <Arduino.h>
#include <Wire.h>

const int SDA_PIN = 21;
const int SCL_PIN = 22;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("===============================");
  Serial.println("I2C Scanner - TOF Validation");
  Serial.println("===============================");
}

void loop() {
  byte error;
  byte address;
  int devices = 0;

  Serial.println("Scanning I2C bus...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      devices++;
    }
  }

  if (devices == 0) {
    Serial.println("No I2C devices found.");
  }

  Serial.println();
  delay(3000);
}
