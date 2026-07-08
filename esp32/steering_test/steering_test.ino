#include <ESP32Servo.h>

Servo steeringServo;

#define SERVO_PIN 13

// عدلي هاي القيم حسب معايرة سيارتك
int CENTER = 70;
int LEFT = 10;
int RIGHT = 130;

void setup() {

  steeringServo.attach(SERVO_PIN);

  // بداية: العجلات مستقيمة
  steeringServo.write(CENTER);

  delay(1000);
}


void loop() {

  // مثال حركة اختبار
  turnLeft();
  delay(1000);

  goStraight();
  delay(1000);

  turnRight();
  delay(1000);

  goStraight();
  delay(1000);
}


// -------- Steering Functions --------

void turnLeft() {
  steeringServo.write(LEFT);
}


void turnRight() {
  steeringServo.write(RIGHT);
}


void goStraight() {
  steeringServo.write(CENTER);
}
