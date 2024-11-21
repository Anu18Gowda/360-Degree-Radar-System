#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

Servo myServo;
bool use360Mode = true; // Set to true for 360° mode, false for 180° mode

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}

void loop() {
  if (use360Mode) {
    // 0 to 180 degrees
    for (int angle = 0; angle <= 180; angle++) {
      scanAndSend(angle);
    }
    // 181 to 360 degrees (mapped to 179 to 0)
    for (int angle = 179; angle >= 0; angle--) {
      scanAndSend(angle + 181);
    }
  } else {
    // 180-degree mode
    for (int angle = 0; angle <= 180; angle++) {
      scanAndSend(angle);
    }
    for (int angle = 179; angle > 0; angle--) {
      scanAndSend(angle);
    }
  }
}

void scanAndSend(int angle) {
  int servoAngle = angle;
  if (angle > 180) {
    servoAngle = 360 - angle; // Map 181-360 to 179-0 for servo
  }
  myServo.write(servoAngle);
  delay(15); // adjust delay to control scan speed

  int distance = getDistance();
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.println(".");
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
