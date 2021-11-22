#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <math.h>

#define SERVOMIN 150
#define SERVOMAX 600
#define SERVO_FREQ 50

#define EYE_INTERVAL_FT 1
#define DISTANCE_TO_PARTICIPANT_FT 4
#define CAMERA_MAX_ANGLE_RAD 1.9

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  for (uint8_t i = 0; i < 16; ++i) {
    pwm.setPWM(i, 0, map(90, 0, 180, SERVOMIN, SERVOMAX));
  }
  delay(500);
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
        angle = 90 + (atan((distanceToCenterFt - 1.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 1:
        angle = 90 + (atan((distanceToCenterFt - 0.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 2:
        angle = 90 + (atan((distanceToCenterFt - 0.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
      case 3:
        angle = 90 + (atan((distanceToCenterFt - 1.5) / (EYE_INTERVAL_FT * 4)) *
                      180 / PI);
        break;
    }
    pwm.setPWM(i, 0, map(angle, 0, 180, SERVOMIN, SERVOMAX));
    Serial.print(String(i) + ": " + (angle) + ", ");
  }
  Serial.println("");

  delay(500);
}