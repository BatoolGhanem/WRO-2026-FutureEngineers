#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

const int SDA_PIN = 21;
const int SCL_PIN = 22;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X
);

String detectColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
    if (c < 50) return "DARK / NO SURFACE";

    float redRatio = (float)r / c;
    float greenRatio = (float)g / c;
    float blueRatio = (float)b / c;

    if (redRatio > 0.45 && greenRatio < 0.35 && blueRatio < 0.35) {
        return "RED";
    }

    if (greenRatio > 0.42 && redRatio < 0.40 && blueRatio < 0.40) {
        return "GREEN";
    }

    if (blueRatio > 0.40 && redRatio < 0.40 && greenRatio < 0.45) {
        return "BLUE";
    }

    if (redRatio > 0.28 && greenRatio > 0.28 && blueRatio > 0.28) {
        return "WHITE";
    }

    return "UNKNOWN";
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("===============================");
    Serial.println("TCS34725 Color Sensor Test");
    Serial.println("===============================");

    Wire.begin(SDA_PIN, SCL_PIN);

    if (!tcs.begin()) {
        Serial.println("ERROR: TCS34725 not detected.");
        Serial.println("Check VIN, GND, SDA, and SCL wiring.");
        while (true) {
            delay(1000);
        }
    }

    Serial.println("TCS34725 detected successfully.");
}

void loop() {
    uint16_t r, g, b, c;

    tcs.getRawData(&r, &g, &b, &c);

    String colorName = detectColor(r, g, b, c);

    Serial.print("R: ");
    Serial.print(r);
    Serial.print(" | G: ");
    Serial.print(g);
    Serial.print(" | B: ");
    Serial.print(b);
    Serial.print(" | Clear: ");
    Serial.print(c);
    Serial.print(" | Detected: ");
    Serial.println(colorName);

    delay(500);
}
