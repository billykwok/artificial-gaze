#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <math.h>

#define ANGLE_MIN 35
#define ANGLE_MAX 145
#define SERVO_FREQ 50

#define EYE_INTERVAL_FT 1
#define DISTANCE_TO_PARTICIPANT_FT 4
#define CAMERA_MAX_ANGLE_RAD 1.92
#define SERVO_DELAY 150

const static uint16_t SERVO_MIN[16] = {
    75,  75,  55,  55,   // row 1
    110, 110, 110, 110,  // row 2
    90,  90,  90,  90,   // row 3
    45,  100, 55,  55    // row 4
};

const static uint16_t SERVO_MAX[16] = {
    440, 450, 465, 500,  // row 1
    465, 465, 465, 465,  // row 2
    465, 465, 465, 465,  // row 3
    440, 465, 465, 460   // row 4
};

long mapAngle(int input, int outputMin, int outputMax) {
  return map(constrain(input, ANGLE_MIN, ANGLE_MAX), 0, 180, outputMin,
             outputMax);
}

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  // while (1) {
  //   for (uint8_t i = 0; i < 16; ++i) {
  //     pwm.setPWM(i, 0, mapAngle(ANGLE_MIN, SERVO_MIN[i], SERVO_MAX[i]));
  //   }
  //   delay(SERVO_DELAY * 50);
  //   for (uint8_t i = 0; i < 16; ++i) {
  //     pwm.setPWM(i, 0, mapAngle(ANGLE_MAX, SERVO_MIN[i], SERVO_MAX[i]));
  //   }
  //   delay(SERVO_DELAY * 50);
  // }
  for (uint8_t i = 0; i < 16; ++i) {
    pwm.setPWM(i, 90, mapAngle(ANGLE_MAX, SERVO_MIN[i], SERVO_MAX[i]));
  }
  delay(SERVO_DELAY);
}

void loop() {
  if (Serial.available() <= 0) return;
  double x = 127.5;
  while (Serial.available() > 0) {
    x = Serial.read();
  }
  Serial.println("x: " + String(x));

  double distanceToCenterFt =
      ((255 - x) / 127.5 - 1) * 4 * tan(CAMERA_MAX_ANGLE_RAD / 2);
  Serial.println("distanceToCenterFt: " + String(distanceToCenterFt));

  double angle;
  for (uint8_t i = 0; i < 16; ++i) {
    angle = 90;
    switch (i % 4) {
      case 0:
        angle = 90 - (atan((distanceToCenterFt - 1.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 1:
        angle = 90 - (atan((distanceToCenterFt - 0.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 2:
        angle = 90 - (atan((distanceToCenterFt - 0.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 3:
        angle = 90 - (atan((distanceToCenterFt - 1.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
    }
    pwm.setPWM(i, 0, mapAngle(angle, SERVO_MIN[i], SERVO_MAX[i]));
    Serial.print(String(i) + ": " + (angle) + ", ");
  }
  Serial.println("");

  delay(SERVO_DELAY);
}