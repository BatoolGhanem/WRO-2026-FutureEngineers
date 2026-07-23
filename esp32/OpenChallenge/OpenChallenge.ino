/*
  WRO Future Engineers 2026
  OPEN CHALLENGE
  FIXED CORNER DETECTION + SAFE POST-TURN ALIGNMENT
*/

#include <Wire.h>
#include <math.h>
#include <Adafruit_VL53L1X.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>

namespace BuildConfig {
  constexpr const char* VERSION = "WRO-FE-2026-POST-TURN-FIX";

  constexpr bool ENABLE_DYNAMIC_BRAKE = false;
  constexpr uint32_t DYNAMIC_BRAKE_DURATION_MS = 55;
}

namespace Pins {
  constexpr uint8_t MAIN_SDA = 21;
  constexpr uint8_t MAIN_SCL = 22;

  constexpr uint8_t LEFT_XSHUT  = 18;
  constexpr uint8_t RIGHT_XSHUT = 19;
  constexpr int8_t TOF_IRQ = -1;

  constexpr uint8_t COLOR_SDA = 16;
  constexpr uint8_t COLOR_SCL = 17;

  constexpr uint8_t MOTOR_ENA = 25;
  constexpr uint8_t MOTOR_IN1 = 26;
  constexpr uint8_t MOTOR_IN2 = 27;

  constexpr uint8_t SERVO = 13;
}

namespace SteeringConfig {
  constexpr int CENTER = 70;

  // الحدود الميكانيكية القصوى للسيرفو
  constexpr int HARD_LEFT  = 25;
  constexpr int HARD_RIGHT = 115;

  /*
    حدود التتبع العادي.
    لا نسمح للسيرفو أثناء السير العادي أن يصل لليمين القوي
    حتى لا يعلق أو يصدر صوت زن.
  */
  constexpr int FOLLOW_LEFT_LIMIT  = 52;
  constexpr int FOLLOW_RIGHT_LIMIT = 86;

  // حدود التمركز بين الحائطين بعد اللفة
  constexpr int ALIGN_LEFT_LIMIT  = 54;
  constexpr int ALIGN_RIGHT_LIMIT = 84;

  // زوايا اللفة نفسها
  constexpr int TURN_LEFT  = 30;
  constexpr int TURN_RIGHT = 112;

  constexpr uint16_t SERVO_MIN_PULSE_US = 500;
  constexpr uint16_t SERVO_MAX_PULSE_US = 2500;

  // سرعة انتقال السيرفو بين الزوايا
  constexpr float MAX_STEERING_RATE_DEG_PER_SEC = 1500.0f;
}

namespace MotorConfig {
  constexpr uint32_t PWM_FREQUENCY_HZ = 15000;
  constexpr uint8_t PWM_RESOLUTION_BITS = 8;

  constexpr int PWM_MIN = 0;
  constexpr int PWM_MAX = 255;

  constexpr int MIN_EFFECTIVE_PWM = 175;

  constexpr int START_SPEED = 178;
  constexpr int CRUISE_SPEED = 176;

  constexpr int TURN_FAST_SPEED = 185;
  constexpr int TURN_SLOW_SPEED = 176;

  constexpr int WALL_RECOVERY_SPEED = 176;

  constexpr int ACCEL_STEP_PWM = 8;
  constexpr int DECEL_STEP_PWM = 10;

  constexpr uint32_t RAMP_INTERVAL_MS = 8;
}

namespace DistanceConfig {
  constexpr int DEFAULT_WALL_DISTANCE_MM = 250;

  constexpr int MIN_VALID_MM = 30;
  constexpr int MAX_VALID_MM = 2500;

  constexpr uint32_t SENSOR_STALE_TIMEOUT_MS = 400;

  constexpr uint8_t MEDIAN_WINDOW = 3;

  // فلتر التتبع العادي
  constexpr float FILTER_ALPHA = 0.38f;

  // فلتر سريع لاكتشاف الفراغ
  constexpr float OPENING_FILTER_ALPHA = 0.88f;

  /*
    تتبع الحائط.
    خففنا KD حتى لا تضرب قراءة الدخلة السيرفو بقوة.
  */
  constexpr float WALL_KP = 0.065f;
  constexpr float WALL_KD = 0.008f;

  constexpr int WALL_ERROR_DEADBAND_MM = 10;

  /*
    تمركز الروبوت بين الحائطين بعد اللفة.
  */
  constexpr float CORRIDOR_CENTER_KP = 0.060f;
  constexpr int CORRIDOR_CENTER_DEADBAND_MM = 15;
  constexpr float MAX_CORRIDOR_CORRECTION_DEG = 14.0f;
}

namespace CornerConfig {
  // مقدار ارتفاع قراءة الحائط حتى نعتبره فراغًا
  constexpr int MIN_RELATIVE_OPENING_RISE_MM = 70;

  // منع اكتشاف نفس الفراغ مرتين
  constexpr uint32_t CORNER_LOCKOUT_MS = 800;

  /*
    وقت قصير بعد اللفة يبقى فيه السيرفو سنتر
    حتى يخرج الروبوت من زاوية الدخلة.
  */
  constexpr uint32_t BLIND_EXIT_DURATION_MS = 170;

  // أقصى وقت للبحث عن الحائط الجديد
  constexpr uint32_t WALL_RECOVERY_TIMEOUT_MS = 2000;

  /*
    مدة التمركز بعد رؤية الحائط الجديد.
    كانت 150ms وهي قصيرة جدًا.
  */
  constexpr uint32_t ALIGN_DURATION_MS = 500;

  /*
    أي قراءة أبعد من هذه القيمة لا نعتبرها حائطًا جديدًا.
    غالبًا تكون فراغًا أو قراءة زاوية غير مستقرة.
  */
  constexpr int NEW_WALL_MAX_DISTANCE_MM = 650;

  // عدد القراءات الجديدة المطلوبة قبل دخول الاستعدال
  constexpr uint8_t NEW_WALL_CONFIRMATIONS = 3;
}

namespace TurnConfig {
  constexpr float TARGET_YAW_DEG = 88.0f;
  constexpr float SLOWDOWN_START_DEG = 75.0f;

  constexpr float BRAKE_PREDICTION_TIME_SEC = 0.035f;

  constexpr float MAX_EXIT_RATE_DPS = 25.0f;
  constexpr float MIN_ACCEPTABLE_YAW_DEG = 85.0f;

  constexpr uint8_t SETTLE_CONFIRMATIONS = 2;

  constexpr uint32_t TURN_HARD_LIMIT_MS = 3500;
}

TwoWire colorWire(1);

Adafruit_VL53L1X leftSensor(
  Pins::LEFT_XSHUT,
  Pins::TOF_IRQ
);

Adafruit_VL53L1X rightSensor(
  Pins::RIGHT_XSHUT,
  Pins::TOF_IRQ
);

Adafruit_MPU6050 mpu;

Adafruit_TCS34725 colorSensor(
  TCS34725_INTEGRATIONTIME_24MS,
  TCS34725_GAIN_4X
);

Servo steeringServo;

enum class RobotState : uint8_t {
  Booting,
  FollowingWall,
  TurningCorner,
  FindingNewWall,
  AligningToWall,
  EmergencyStop
};

struct DistanceReading {
  int rawMm = -1;

  float filteredMm = NAN;
  float previousFilteredMm = NAN;
  float rateMmPerSec = 0.0f;

  float openingFilteredMm = NAN;

  int history[DistanceConfig::MEDIAN_WINDOW] = {0};

  uint8_t historyCount = 0;
  uint8_t historyIndex = 0;

  uint32_t timestampMs = 0;

  bool valid = false;
  bool fresh = false;
};

struct ImuRuntime {
  bool ready = false;
  bool calibrated = false;

  float gyroBiasRadPerSec = 0.0f;

  float yawDeg = 0.0f;
  float angularVelocityDps = 0.0f;

  uint32_t lastUpdateMicros = 0;
  uint32_t lastGoodUpdateMs = 0;
};

struct SteeringRuntime {
  float targetAngle = SteeringConfig::CENTER;
  float appliedAngle = SteeringConfig::CENTER;

  uint32_t lastUpdateMs = 0;
};

struct CornerRuntime {
  float stableWallBaselineMm = NAN;

  uint32_t turnStartMs = 0;
  uint32_t lastCornerMs = 0;

  bool turnLeft = false;
  bool turnBrakingActive = false;

  uint8_t turnSettleReadings = 0;
  uint8_t newWallReadings = 0;

  /*
    نخزن توقيت آخر قراءة احتسبناها،
    حتى لا نعد نفس قراءة الحساس أكثر من مرة.
  */
  uint32_t lastNewWallSampleMs = 0;
};

struct MotorRuntime {
  int targetPwm = 0;
  int appliedPwm = 0;

  uint32_t lastRampUpdateMs = 0;
  uint32_t brakeUntilMs = 0;

  bool braking = false;
};

struct RobotRuntime {
  RobotState state = RobotState::Booting;

  DistanceReading leftDistance;
  DistanceReading rightDistance;

  ImuRuntime imu;
  SteeringRuntime steering;
  CornerRuntime corner;
  MotorRuntime motor;

  bool innerWallIsLeft = true;

  uint32_t stateEnteredMs = 0;
};

RobotRuntime robot;

bool isDistanceValid(int distanceMm) {
  return distanceMm >= DistanceConfig::MIN_VALID_MM &&
         distanceMm <= DistanceConfig::MAX_VALID_MM;
}

bool isDistanceFresh(const DistanceReading& reading) {
  return reading.valid &&
         (millis() - reading.timestampMs <=
          DistanceConfig::SENSOR_STALE_TIMEOUT_MS);
}

const DistanceReading& followedWallReadingConst() {
  return robot.innerWallIsLeft
    ? robot.leftDistance
    : robot.rightDistance;
}

int clampSteeringAngle(int angle) {
  return constrain(
    angle,
    SteeringConfig::HARD_LEFT,
    SteeringConfig::HARD_RIGHT
  );
}

void setSteeringImmediate(int angle) {
  const int safeAngle = clampSteeringAngle(angle);

  robot.steering.targetAngle = safeAngle;
  robot.steering.appliedAngle = safeAngle;
  robot.steering.lastUpdateMs = millis();

  steeringServo.write(safeAngle);
}

void setSteeringTarget(int angle) {
  robot.steering.targetAngle =
    clampSteeringAngle(angle);
}

void steerCenter() {
  setSteeringTarget(SteeringConfig::CENTER);
}

void updateSteeringActuator() {
  const uint32_t nowMs = millis();

  if (robot.steering.lastUpdateMs == 0) {
    robot.steering.lastUpdateMs = nowMs;
    return;
  }

  float dtSeconds = constrain(
    (nowMs - robot.steering.lastUpdateMs) / 1000.0f,
    0.0f,
    0.05f
  );

  robot.steering.lastUpdateMs = nowMs;

  const float maxStep =
    SteeringConfig::MAX_STEERING_RATE_DEG_PER_SEC *
    dtSeconds;

  const float error =
    robot.steering.targetAngle -
    robot.steering.appliedAngle;

  if (fabsf(error) <= maxStep) {
    robot.steering.appliedAngle =
      robot.steering.targetAngle;
  }
  else if (error > 0.0f) {
    robot.steering.appliedAngle += maxStep;
  }
  else {
    robot.steering.appliedAngle -= maxStep;
  }

  steeringServo.write(
    static_cast<int>(
      roundf(robot.steering.appliedAngle)
    )
  );
}

void enterState(RobotState newState) {
  robot.state = newState;
  robot.stateEnteredMs = millis();
}

uint32_t timeInCurrentStateMs() {
  return millis() - robot.stateEnteredMs;
}

int clampMotorPwm(int pwm) {
  return constrain(
    pwm,
    MotorConfig::PWM_MIN,
    MotorConfig::PWM_MAX
  );
}

void applyMotorPwmImmediate(int pwm) {
  pwm = clampMotorPwm(pwm);

  if (pwm <= 0) {
    ledcWrite(Pins::MOTOR_ENA, 0);

    digitalWrite(Pins::MOTOR_IN1, LOW);
    digitalWrite(Pins::MOTOR_IN2, LOW);

    robot.motor.appliedPwm = 0;
    return;
  }

  digitalWrite(Pins::MOTOR_IN1, HIGH);
  digitalWrite(Pins::MOTOR_IN2, LOW);

  ledcWrite(Pins::MOTOR_ENA, pwm);

  robot.motor.appliedPwm = pwm;
}

void setMotorTarget(int pwm) {
  pwm = clampMotorPwm(pwm);

  robot.motor.targetPwm =
    (pwm <= 0)
      ? 0
      : max(pwm, MotorConfig::MIN_EFFECTIVE_PWM);
}

void motorForward(int pwm) {
  setMotorTarget(pwm);
}

void motorCoastStop() {
  robot.motor.targetPwm = 0;
  robot.motor.braking = false;

  applyMotorPwmImmediate(0);
}

void motorBrakeStop() {
  robot.motor.targetPwm = 0;

  if (!BuildConfig::ENABLE_DYNAMIC_BRAKE) {
    motorCoastStop();
    return;
  }

  robot.motor.braking = true;

  robot.motor.brakeUntilMs =
    millis() +
    BuildConfig::DYNAMIC_BRAKE_DURATION_MS;

  digitalWrite(Pins::MOTOR_IN1, HIGH);
  digitalWrite(Pins::MOTOR_IN2, HIGH);

  ledcWrite(Pins::MOTOR_ENA, 255);

  robot.motor.appliedPwm = 0;
}

void updateMotorActuator() {
  const uint32_t nowMs = millis();

  if (robot.motor.braking) {
    if (
      static_cast<int32_t>(
        nowMs - robot.motor.brakeUntilMs
      ) < 0
    ) {
      return;
    }

    robot.motor.braking = false;
    applyMotorPwmImmediate(0);
  }

  if (robot.motor.lastRampUpdateMs == 0) {
    robot.motor.lastRampUpdateMs = nowMs;
    return;
  }

  if (
    nowMs - robot.motor.lastRampUpdateMs <
    MotorConfig::RAMP_INTERVAL_MS
  ) {
    return;
  }

  robot.motor.lastRampUpdateMs = nowMs;

  int current = robot.motor.appliedPwm;
  const int target = robot.motor.targetPwm;

  if (current == target) {
    return;
  }

  if (current == 0 && target > 0) {
    current = min(
      target,
      MotorConfig::MIN_EFFECTIVE_PWM
    );

    applyMotorPwmImmediate(current);
    return;
  }

  if (current < target) {
    current = min(
      target,
      current + MotorConfig::ACCEL_STEP_PWM
    );
  }
  else {
    current = max(
      target,
      current - MotorConfig::DECEL_STEP_PWM
    );
  }

  applyMotorPwmImmediate(current);
}

int calculateMedian(
  const int* values,
  uint8_t count
) {
  if (count == 0) {
    return -1;
  }

  int sorted[DistanceConfig::MEDIAN_WINDOW];

  for (uint8_t i = 0; i < count; i++) {
    sorted[i] = values[i];
  }

  for (uint8_t i = 1; i < count; i++) {
    int key = sorted[i];
    int8_t j = i - 1;

    while (j >= 0 && sorted[j] > key) {
      sorted[j + 1] = sorted[j];
      j--;
    }

    sorted[j + 1] = key;
  }

  return sorted[count / 2];
}

void pushDistanceSample(
  DistanceReading& reading,
  int distanceMm,
  uint32_t nowMs
) {
  reading.rawMm = distanceMm;

  if (!isDistanceValid(distanceMm)) {
    return;
  }

  reading.history[reading.historyIndex] =
    distanceMm;

  reading.historyIndex =
    (reading.historyIndex + 1) %
    DistanceConfig::MEDIAN_WINDOW;

  if (
    reading.historyCount <
    DistanceConfig::MEDIAN_WINDOW
  ) {
    reading.historyCount++;
  }

  int medianMm = calculateMedian(
    reading.history,
    reading.historyCount
  );

  if (medianMm < 0) {
    return;
  }

  float oldFiltered = reading.filteredMm;
  uint32_t oldTimestamp = reading.timestampMs;

  reading.previousFilteredMm = oldFiltered;

  if (isnan(oldFiltered)) {
    reading.filteredMm = medianMm;
    reading.rateMmPerSec = 0.0f;
  }
  else {
    reading.filteredMm =
      DistanceConfig::FILTER_ALPHA * medianMm +
      (1.0f - DistanceConfig::FILTER_ALPHA) *
      oldFiltered;

    float dt =
      (nowMs - oldTimestamp) / 1000.0f;

    reading.rateMmPerSec =
      (dt > 0.001f)
        ? (reading.filteredMm - oldFiltered) / dt
        : 0.0f;
  }

  if (isnan(reading.openingFilteredMm)) {
    reading.openingFilteredMm = distanceMm;
  }
  else {
    reading.openingFilteredMm =
      DistanceConfig::OPENING_FILTER_ALPHA *
      distanceMm +
      (1.0f -
       DistanceConfig::OPENING_FILTER_ALPHA) *
      reading.openingFilteredMm;
  }

  reading.timestampMs = nowMs;
  reading.valid = true;
  reading.fresh = true;
}

void updateTofSensors() {
  if (leftSensor.dataReady()) {
    pushDistanceSample(
      robot.leftDistance,
      leftSensor.distance(),
      millis()
    );

    leftSensor.clearInterrupt();
  }

  if (rightSensor.dataReady()) {
    pushDistanceSample(
      robot.rightDistance,
      rightSensor.distance(),
      millis()
    );

    rightSensor.clearInterrupt();
  }

  robot.leftDistance.fresh =
    isDistanceFresh(robot.leftDistance);

  robot.rightDistance.fresh =
    isDistanceFresh(robot.rightDistance);
}

bool readGyroZRadPerSec(float& gyroZ) {
  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;

  mpu.getEvent(&a, &g, &temp);

  gyroZ = g.gyro.z;

  return isfinite(gyroZ);
}

bool calibrateGyroZ() {
  float gyroZ = 0.0f;

  for (uint16_t i = 0; i < 50; i++) {
    readGyroZRadPerSec(gyroZ);
    delay(3);
  }

  double mean = 0.0;

  for (uint16_t i = 1; i <= 300; i++) {
    readGyroZRadPerSec(gyroZ);

    mean += (gyroZ - mean) / i;

    delay(3);
  }

  robot.imu.gyroBiasRadPerSec = mean;
  robot.imu.calibrated = true;

  return true;
}

bool initializeMpu() {
  if (!mpu.begin(0x68, &Wire)) {
    return false;
  }

  mpu.setAccelerometerRange(
    MPU6050_RANGE_4_G
  );

  mpu.setGyroRange(
    MPU6050_RANGE_500_DEG
  );

  mpu.setFilterBandwidth(
    MPU6050_BAND_21_HZ
  );

  delay(100);

  calibrateGyroZ();

  robot.imu.yawDeg = 0.0f;
  robot.imu.lastUpdateMicros = micros();
  robot.imu.lastGoodUpdateMs = millis();
  robot.imu.ready = true;

  return true;
}

void resetYaw() {
  robot.imu.yawDeg = 0.0f;
  robot.imu.angularVelocityDps = 0.0f;
  robot.imu.lastUpdateMicros = micros();
}

void updateImu() {
  if (
    !robot.imu.ready ||
    !robot.imu.calibrated
  ) {
    return;
  }

  uint32_t nowMicros = micros();

  float dt =
    (nowMicros - robot.imu.lastUpdateMicros) /
    1000000.0f;

  robot.imu.lastUpdateMicros = nowMicros;

  if (dt < 0.0005f || dt > 0.080f) {
    return;
  }

  float rawZ = 0.0f;

  if (!readGyroZRadPerSec(rawZ)) {
    return;
  }

  float rateDps =
    (rawZ - robot.imu.gyroBiasRadPerSec) *
    57.2957795f;

  if (fabsf(rateDps) < 0.65f) {
    rateDps = 0.0f;
  }

  robot.imu.angularVelocityDps = rateDps;
  robot.imu.yawDeg += rateDps * dt;
  robot.imu.lastGoodUpdateMs = millis();
}

bool initializeSingleTof(
  Adafruit_VL53L1X& sensor,
  uint8_t address
) {
  if (!sensor.begin(address, &Wire)) {
    return false;
  }

  sensor.setTimingBudget(20);

  return sensor.startRanging();
}

bool initializeTofSensors() {
  pinMode(Pins::LEFT_XSHUT, OUTPUT);
  pinMode(Pins::RIGHT_XSHUT, OUTPUT);

  digitalWrite(Pins::LEFT_XSHUT, LOW);
  digitalWrite(Pins::RIGHT_XSHUT, LOW);

  delay(20);

  if (!initializeSingleTof(leftSensor, 0x30)) {
    return false;
  }

  delay(10);

  if (!initializeSingleTof(rightSensor, 0x31)) {
    return false;
  }

  delay(10);

  return true;
}

float calculateWallSteeringCorrection() {
  const DistanceReading& wall =
    followedWallReadingConst();

  if (
    !isDistanceFresh(wall) ||
    !isfinite(wall.filteredMm)
  ) {
    return 0.0f;
  }

  float errorMm =
    DistanceConfig::DEFAULT_WALL_DISTANCE_MM -
    wall.filteredMm;

  if (
    fabsf(errorMm) <=
    DistanceConfig::WALL_ERROR_DEADBAND_MM
  ) {
    errorMm = 0.0f;
  }

  /*
    تحديد سرعة تغير القراءة حتى لا تسبب قراءة مفاجئة
    أمرًا قويًا للسيرفو.
  */
  float safeRateMmPerSec = constrain(
    wall.rateMmPerSec,
    -500.0f,
    500.0f
  );

  float correctionDeg =
    DistanceConfig::WALL_KP * errorMm -
    DistanceConfig::WALL_KD *
    safeRateMmPerSec;

  return constrain(
    correctionDeg,
    -16.0f,
    16.0f
  );
}

float calculateCorridorCenteringCorrection() {
  if (
    !isDistanceFresh(robot.leftDistance) ||
    !isDistanceFresh(robot.rightDistance)
  ) {
    return 0.0f;
  }

  if (
    !isfinite(robot.leftDistance.filteredMm) ||
    !isfinite(robot.rightDistance.filteredMm)
  ) {
    return 0.0f;
  }

  /*
    موجب: الحائط اليسار أبعد من الحائط اليمين،
    أي أن الروبوت أقرب إلى اليمين ويجب أن يتجه يسارًا.

    سالب: الحائط اليمين أبعد،
    أي أن الروبوت أقرب إلى اليسار ويجب أن يتجه يمينًا.
  */
  float corridorErrorMm =
    robot.leftDistance.filteredMm -
    robot.rightDistance.filteredMm;

  if (
    fabsf(corridorErrorMm) <=
    DistanceConfig::CORRIDOR_CENTER_DEADBAND_MM
  ) {
    corridorErrorMm = 0.0f;
  }

  float correctionDeg =
    -DistanceConfig::CORRIDOR_CENTER_KP *
    corridorErrorMm;

  return constrain(
    correctionDeg,
    -DistanceConfig::MAX_CORRIDOR_CORRECTION_DEG,
    DistanceConfig::MAX_CORRIDOR_CORRECTION_DEG
  );
}

void followInnerWall(int speedPwm) {
  const DistanceReading& wall =
    followedWallReadingConst();

  if (
    !isDistanceFresh(wall) ||
    !isfinite(wall.filteredMm)
  ) {
    /*
      عند فقدان القراءة:
      نرجع السيرفو سنتر ونوقف المحرك بدل أن يبقى
      السيرفو مشدودًا يمين أو يسار.
    */
    setSteeringImmediate(
      SteeringConfig::CENTER
    );

    motorCoastStop();
    return;
  }

  float correctionDeg =
    calculateWallSteeringCorrection();

  float angle;

  if (robot.innerWallIsLeft) {
    angle =
      SteeringConfig::CENTER +
      correctionDeg;
  }
  else {
    angle =
      SteeringConfig::CENTER -
      correctionDeg;
  }

  /*
    هذه أهم حماية:
    حدود التتبع أضيق من الحدود الميكانيكية.
  */
  angle = constrain(
    angle,
    SteeringConfig::FOLLOW_LEFT_LIMIT,
    SteeringConfig::FOLLOW_RIGHT_LIMIT
  );

  setSteeringTarget(
    static_cast<int>(roundf(angle))
  );

  motorForward(speedPwm);
}

void beginCornerTurn() {
  robot.corner.turnLeft =
    robot.innerWallIsLeft;

  robot.corner.turnStartMs = millis();
  robot.corner.turnSettleReadings = 0;
  robot.corner.turnBrakingActive = false;

  resetYaw();

  setSteeringImmediate(
    robot.corner.turnLeft
      ? SteeringConfig::TURN_LEFT
      : SteeringConfig::TURN_RIGHT
  );

  applyMotorPwmImmediate(
    MotorConfig::TURN_FAST_SPEED
  );

  enterState(
    RobotState::TurningCorner
  );
}

void completeCornerTurn() {
  robot.corner.lastCornerMs = millis();
  robot.corner.stableWallBaselineMm = NAN;

  robot.corner.newWallReadings = 0;
  robot.corner.lastNewWallSampleMs = 0;

  /*
    إلغاء تأثير سرعة تغير القراءة القديمة،
    لأن القراءة قبل اللفة لا تمثل الحائط الجديد.
  */
  robot.leftDistance.rateMmPerSec = 0.0f;
  robot.rightDistance.rateMmPerSec = 0.0f;

  robot.leftDistance.previousFilteredMm =
    robot.leftDistance.filteredMm;

  robot.rightDistance.previousFilteredMm =
    robot.rightDistance.filteredMm;

  // رجوع فوري للسيرفو إلى السنتر
  setSteeringImmediate(
    SteeringConfig::CENTER
  );

  applyMotorPwmImmediate(
    MotorConfig::WALL_RECOVERY_SPEED
  );

  enterState(
    RobotState::FindingNewWall
  );
}

void updateCornerTurn() {
  if (
    robot.state !=
    RobotState::TurningCorner
  ) {
    return;
  }

  /*
    حماية إذا تعطلت قراءة الجايرو أو لم تكتمل اللفة.
  */
  if (
    timeInCurrentStateMs() >=
    TurnConfig::TURN_HARD_LIMIT_MS
  ) {
    completeCornerTurn();
    return;
  }

  float angleDeg =
    fabsf(robot.imu.yawDeg);

  float turnRateDps =
    fabsf(robot.imu.angularVelocityDps);

  if (!robot.corner.turnBrakingActive) {
    float predictedAngle =
      angleDeg +
      turnRateDps *
      TurnConfig::BRAKE_PREDICTION_TIME_SEC;

    if (
      predictedAngle >=
      TurnConfig::TARGET_YAW_DEG
    ) {
      robot.corner.turnBrakingActive = true;

      motorBrakeStop();

      setSteeringImmediate(
        SteeringConfig::CENTER
      );

      return;
    }

    if (
      angleDeg >=
      TurnConfig::SLOWDOWN_START_DEG
    ) {
      motorForward(
        MotorConfig::TURN_SLOW_SPEED
      );
    }
    else {
      motorForward(
        MotorConfig::TURN_FAST_SPEED
      );
    }

    return;
  }

  if (
    angleDeg >=
      TurnConfig::MIN_ACCEPTABLE_YAW_DEG &&
    turnRateDps <=
      TurnConfig::MAX_EXIT_RATE_DPS
  ) {
    robot.corner.turnSettleReadings++;

    if (
      robot.corner.turnSettleReadings >=
      TurnConfig::SETTLE_CONFIRMATIONS
    ) {
      completeCornerTurn();
    }
  }
  else {
    robot.corner.turnSettleReadings = 0;
  }
}

void updateFindingNewWall() {
  if (
    robot.state !=
    RobotState::FindingNewWall
  ) {
    return;
  }

  setSteeringTarget(
    SteeringConfig::CENTER
  );

  motorForward(
    MotorConfig::WALL_RECOVERY_SPEED
  );

  uint32_t timeInState =
    timeInCurrentStateMs();

  /*
    وقت قصير يخرج فيه الروبوت مستقيمًا
    بعد نهاية اللفة.
  */
  if (
    timeInState <
    CornerConfig::BLIND_EXIT_DURATION_MS
  ) {
    return;
  }

  const DistanceReading& wall =
    followedWallReadingConst();

  if (
    !isDistanceFresh(wall) ||
    !isfinite(wall.filteredMm)
  ) {
    robot.corner.newWallReadings = 0;

    if (
      timeInState >=
      CornerConfig::WALL_RECOVERY_TIMEOUT_MS
    ) {
      enterState(
        RobotState::AligningToWall
      );
    }

    return;
  }

  /*
    لا نقبل قراءة من قبل دخول حالة البحث،
    ولا نكرر احتساب نفس العينة.
  */
  bool isNewSample =
    wall.timestampMs >
      robot.stateEnteredMs &&
    wall.timestampMs !=
      robot.corner.lastNewWallSampleMs;

  if (isNewSample) {
    robot.corner.lastNewWallSampleMs =
      wall.timestampMs;

    if (
      wall.filteredMm <=
      CornerConfig::NEW_WALL_MAX_DISTANCE_MM
    ) {
      robot.corner.newWallReadings++;

      if (
        robot.corner.newWallReadings >=
        CornerConfig::NEW_WALL_CONFIRMATIONS
      ) {
        robot.corner.newWallReadings = 0;

        enterState(
          RobotState::AligningToWall
        );

        return;
      }
    }
    else {
      robot.corner.newWallReadings = 0;
    }
  }

  /*
    حماية حتى لا يبقى الروبوت في البحث للأبد.
  */
  if (
    timeInState >=
    CornerConfig::WALL_RECOVERY_TIMEOUT_MS
  ) {
    robot.corner.newWallReadings = 0;

    enterState(
      RobotState::AligningToWall
    );
  }
}

void updateWallAlignment() {
  if (
    robot.state !=
    RobotState::AligningToWall
  ) {
    return;
  }

  bool bothWallsVisible =
    isDistanceFresh(robot.leftDistance) &&
    isDistanceFresh(robot.rightDistance) &&
    isfinite(robot.leftDistance.filteredMm) &&
    isfinite(robot.rightDistance.filteredMm);

  if (bothWallsVisible) {
    /*
      عند رؤية الحائطين:
      يتمركز الروبوت بينهما بدل تتبع حائط واحد.
    */
    float correctionDeg =
      calculateCorridorCenteringCorrection();

    float targetAngle =
      SteeringConfig::CENTER +
      correctionDeg;

    targetAngle = constrain(
      targetAngle,
      SteeringConfig::ALIGN_LEFT_LIMIT,
      SteeringConfig::ALIGN_RIGHT_LIMIT
    );

    setSteeringTarget(
      static_cast<int>(
        roundf(targetAngle)
      )
    );

    motorForward(
      MotorConfig::WALL_RECOVERY_SPEED
    );
  }
  else {
    /*
      إذا لم يرَ الحائطين معًا،
      يتبع الحائط الداخلي ضمن الحدود الآمنة.
    */
    followInnerWall(
      MotorConfig::WALL_RECOVERY_SPEED
    );
  }

  if (
    timeInCurrentStateMs() >=
    CornerConfig::ALIGN_DURATION_MS
  ) {
    /*
      تصفير مشتقة المسافة قبل العودة للتتبع العادي.
    */
    robot.leftDistance.rateMmPerSec = 0.0f;
    robot.rightDistance.rateMmPerSec = 0.0f;

    robot.leftDistance.previousFilteredMm =
      robot.leftDistance.filteredMm;

    robot.rightDistance.previousFilteredMm =
      robot.rightDistance.filteredMm;

    enterState(
      RobotState::FollowingWall
    );

    motorForward(
      MotorConfig::CRUISE_SPEED
    );
  }
}

void setup() {
  Serial.begin(115200);

  delay(500);

  Serial.println();
  Serial.println(BuildConfig::VERSION);

  pinMode(
    Pins::MOTOR_IN1,
    OUTPUT
  );

  pinMode(
    Pins::MOTOR_IN2,
    OUTPUT
  );

  pinMode(
    Pins::MOTOR_ENA,
    OUTPUT
  );

  ESP32PWM::allocateTimer(0);

  steeringServo.setPeriodHertz(50);

  steeringServo.attach(
    Pins::SERVO,
    SteeringConfig::SERVO_MIN_PULSE_US,
    SteeringConfig::SERVO_MAX_PULSE_US
  );

  setSteeringImmediate(
    SteeringConfig::CENTER
  );

  ledcAttach(
    Pins::MOTOR_ENA,
    MotorConfig::PWM_FREQUENCY_HZ,
    MotorConfig::PWM_RESOLUTION_BITS
  );

  applyMotorPwmImmediate(0);

  Wire.begin(
    Pins::MAIN_SDA,
    Pins::MAIN_SCL,
    100000
  );

  bool tofReady =
    initializeTofSensors();

  bool mpuReady =
    initializeMpu();

  if (!tofReady || !mpuReady) {
    Serial.println(
      "ERROR: Sensor initialization failed"
    );

    enterState(
      RobotState::EmergencyStop
    );

    motorCoastStop();

    setSteeringImmediate(
      SteeringConfig::CENTER
    );

    return;
  }

  delay(1000);

  /*
    نقرأ عدة مرات بدل قراءة واحدة فقط،
    حتى تمتلئ فلاتر الحساسات قبل البداية.
  */
  uint32_t warmupStartMs = millis();

  while (millis() - warmupStartMs < 500) {
    updateTofSensors();
    delay(5);
  }

  if (
    isDistanceFresh(robot.leftDistance) &&
    isDistanceFresh(robot.rightDistance)
  ) {
    robot.innerWallIsLeft =
      robot.leftDistance.filteredMm <
      robot.rightDistance.filteredMm;
  }

  robot.corner.stableWallBaselineMm = NAN;
  robot.corner.lastCornerMs = 0;

  delay(500);

  resetYaw();

  enterState(
    RobotState::FollowingWall
  );

  motorForward(
    MotorConfig::START_SPEED
  );
}

void loop() {
  updateTofSensors();
  updateImu();

  const DistanceReading& wall =
    followedWallReadingConst();

  /*
    تحديث خط أساس الحائط فقط أثناء التتبع الطبيعي.
  */
  if (
    robot.state ==
      RobotState::FollowingWall &&
    isDistanceFresh(wall) &&
    isfinite(wall.filteredMm)
  ) {
    if (
      isnan(
        robot.corner.stableWallBaselineMm
      )
    ) {
      robot.corner.stableWallBaselineMm =
        wall.filteredMm;
    }
    else {
      robot.corner.stableWallBaselineMm =
        0.90f *
          robot.corner.stableWallBaselineMm +
        0.10f *
          wall.filteredMm;
    }
  }

  switch (robot.state) {
    case RobotState::FollowingWall: {
      followInnerWall(
        MotorConfig::CRUISE_SPEED
      );

      bool lockoutFinished =
        millis() -
          robot.corner.lastCornerMs >
        CornerConfig::CORNER_LOCKOUT_MS;

      if (lockoutFinished) {
        bool readingReady =
          isDistanceFresh(wall) &&
          isfinite(wall.openingFilteredMm) &&
          !isnan(
            robot.corner.stableWallBaselineMm
          );

        if (readingReady) {
          float openingRiseMm =
            wall.openingFilteredMm -
            robot.corner.stableWallBaselineMm;

          if (
            openingRiseMm >=
            CornerConfig::
              MIN_RELATIVE_OPENING_RISE_MM
          ) {
            beginCornerTurn();
          }
        }
      }

      break;
    }

    case RobotState::TurningCorner:
      updateCornerTurn();
      break;

    case RobotState::FindingNewWall:
      updateFindingNewWall();
      break;

    case RobotState::AligningToWall:
      updateWallAlignment();
      break;

    case RobotState::EmergencyStop:
      motorCoastStop();

      setSteeringImmediate(
        SteeringConfig::CENTER
      );

      break;

    default:
      motorCoastStop();

      setSteeringImmediate(
        SteeringConfig::CENTER
      );

      break;
  }

  updateMotorActuator();
  updateSteeringActuator();

  delay(1);
}
