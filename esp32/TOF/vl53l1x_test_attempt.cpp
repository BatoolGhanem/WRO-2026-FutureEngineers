#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L1X.h>

Adafruit_VL53L1X vl53;

const int SDA_PIN = 21;
const int SCL_PIN = 22;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("===============================");
  Serial.println("TOF400C VL53L1X Test Attempt");
  Serial.println("===============================");

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  if (!vl53.begin(0x29, &Wire)) {
    Serial.println("ERROR: VL53L1X library initialization failed.");
    Serial.println("I2C scanner detects the sensor at 0x29, but this library cannot initialize it.");
    Serial.println("This is documented as an engineering issue for future investigation.");
    while (true) delay(1000);
  }

  Serial.println("VL53L1X initialized successfully.");

  if (!vl53.startRanging()) {
    Serial.println("ERROR: Failed to start ranging.");
    while (true) delay(1000);
  }
}

void loop() {
  if (vl53.dataReady()) {
    int16_t distance = vl53.distance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");

    vl53.clearInterrupt();
  }

  delay(50);
}
