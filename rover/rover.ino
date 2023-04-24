// https://lastminuteengineers.com/l293d-motor-driver-shield-arduino-tutorial/
#include <AFMotor.h>
#include <SoftwareSerial.h>

// Drive Instructions
const char *CMD_FORWARD = "forward";
const char *CMD_BACKWARD = "backward";
const char *CMD_STOP = "stop";
const int TERM = '.';

// [f || b][l || r]_motor
AF_DCMotor fr_motor(1);
AF_DCMotor fl_motor(2);

// AF_DCMotor bl_motor(3);
// AF_DCMotor br_motor(4);

#define RXpin 2
#define TXpin 3
SoftwareSerial Serial2(RXpin, TXpin);

char *buffer;

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
  Serial.println("Starting this up!");

  //Set initial speed of the motor & stop
	fl_motor.setSpeed(55);
	fl_motor.run(RELEASE);
  fr_motor.setSpeed(55);
  fr_motor.run(RELEASE);
}

void loop() {

  if (Serial.peek() != -1) {
    String cmd = Serial.readStringUntil(';');
    String left_speed = Serial.readStringUntil(';');
    String right_speed = Serial.readStringUntil(';');
    if (strcmp(cmd.c_str(), CMD_FORWARD) == 0) {
      // Move forward
      Serial2.println("moving forward!");
      fl_motor.run(BACKWARD);
      fl_motor.setSpeed(left_speed)
      fr_motor.run(FORWARD);
      fr_motor.setSpeed(right_speed)
    } else if (strcmp(cmd.c_str(), CMD_BACKWARD) == 0) {
      Serial2.println("moving backward!");
      fl_motor.run(FORWARD);
      fl_motor.setSpeed(left_speed)
      fr_motor.run(BACKWARD);
      fr_motor.setSpeed(right_speed)
    } else if (strcmp(cmd.c_str(), CMD_STOP) == 0) {
      Serial2.println("stopping!");
      fl_motor.run(RELEASE);
      fr_motor.run(RELEASE);
    }
    Serial.println(cmd.c_str());
  }
  // delay(1500);
}


//
// BEGIN #1
//

// // https://github.com/tylersweat/arduino-L293D
// #include <L293D.h>

// // TODO: Figure out correct pin config
// L293D front_left_motor(9,8,7);
// L293D front_right_motor(9,8,7);
// // L293D back_left_motor(9,8,7);
// // L293D back_right_motor(9,8,7);

// // Drive Instructions
// const int CMD_FORWARD = 1;
// const int CMD_BACKWARD = 2;
// const int CMD_STOP = 3;

// // this sample code provided by www.programmingboss.com
// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   int cmd = Serial.read();
//   int speed = Serial.read();
//   if (cmd == CMD_FORWARD) {
//     front_left_motor.set(speed);
//     front_right_motor.set(speed);
//   } else if (cmd == CMD_BACKWARD) {
//     front_left_motor.set(-1*speed);
//     front_right_motor.set(-1*speed);
//   } else if (cmd == CMD_STOP) {
//     front_left_motor.set(0);
//     front_right_motor.set(0);
//   } else {
//     // Unknown command
//   }
//   delay(500);
// }
