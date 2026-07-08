#include <ESP32Servo.h>

Servo steeringServo;

#define SERVO_PIN 13

int CENTER = 70;
int LEFT = 10;
int RIGHT = 130;

void setup() {

  steeringServo.attach(SERVO_PIN);

  steeringServo.write(CENTER);

  delay(1000);
}


void loop() {

  turnLeft();
  delay(1000);

  goStraight();
  delay(1000);

  turnRight();
  delay(1000);

  goStraight();
  delay(1000);
}



void turnLeft() {
  steeringServo.write(LEFT);
}


void turnRight() {
  steeringServo.write(RIGHT);
}


void goStraight() {
  steeringServo.write(CENTER);
}
