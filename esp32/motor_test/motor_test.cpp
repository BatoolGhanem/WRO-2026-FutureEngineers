#include <Arduino.h>

const int IN1 = 25;
const int IN2 = 26;
const int ENA = 27;

void setup()
{
    Serial.begin(115200);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    analogWrite(ENA, 0);

    Serial.println("===============================");
    Serial.println("L298N Motor Test Started");
    Serial.println("===============================");
}

void loop()
{
    Serial.println("Forward");

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 180);

    delay(3000);

    Serial.println("Stop");

    analogWrite(ENA, 0);

    delay(2000);

    Serial.println("Reverse");

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 180);

    delay(3000);

    Serial.println("Stop");

    analogWrite(ENA, 0);

    delay(2000);
}
