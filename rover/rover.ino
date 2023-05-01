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
SoftwareSerial Printer(RXpin, TXpin);

char *buffer;

void setup() {
  Serial.begin(9600);
  Printer.begin(115200);
  Serial.println("Starting this up!");

  //Set initial speed of the motor & stop
	m1.setSpeed(0);
	m1.run(RELEASE);

  m2.setSpeed(0);
  m2.run(RELEASE);

  m3.setSpeed(0);
  m3.run(RELEASE);

  m4.setSpeed(0);
  m4.run(RELEASE);

  current_left_speed = 0;
  current_right_speed = 0;
}

void update_speeds(int target_left, int target_right) {
  m2.setSpeed(target_left);
  m3.setSpeed(target_left);
  current_left_speed = target_left;

  m1.setSpeed(target_right);
  m4.setSpeed(target_right);
  current_right_speed = target_right;
}

void update_directions(int left_dir, int right_dir) {
  if (left_dir == FORWARD) {
    m2.run(FORWARD);
    m3.run(BACKWARD);
  } else if (left_dir == BACKWARD) {
    m2.run(BACKWARD);
    m3.run(FORWARD);
  } else {
    m2.run(RELEASE);
    m3.run(RELEASE);
  }

  if (right_dir == FORWARD) {
    m1.run(FORWARD);
    m4.run(BACKWARD);
  } else if (right_dir == BACKWARD) {
    m1.run(BACKWARD);
    m4.run(FORWARD);
  } else {
    m1.run(RELEASE);
    m4.run(RELEASE);
  }
}

void update_motors(int speed, int direction, int ratio) {

  // Gentle steering
  int target_left_speed = speed; 
  int target_right_speed = speed;

  // Steer right
  if (ratio > 0) {
    target_right_speed = speed * (100 - ratio) / 100;
  }

  // Steer left
  if (ratio < 0) {
    target_left_speed = speed * (100 - ratio * -1) / 100;
  }

  // Default case
  update_directions(direction, direction);
  update_speeds(target_left_speed, target_right_speed);
  return;
}

void loop() {

  if (Serial.peek() != -1) {

    // Parse command
    String message = Serial.readStringUntil('\n');
    int action = message.substring(0, message.indexOf(delimiter)).toInt();
    message.remove(0, message.indexOf(delimiter) + 1);

    Serial.print("Received command");
    Serial.println(action);

    // Handle cut command
    if (action != NULL && action == 3) {
      Serial.println("Doing a cut!");
      int state = message.substring(0, message.indexOf(delimiter)).toInt();
      message.remove(0, message.indexOf(delimiter) + 1);
      if (state != 0 && state != 1) {
        Serial.println("Cutting went terribly wrong :(");
      }
      cut();
      delay(40);
    
    // Handle drive command
    } else {
      int power_ratio = message.substring(0, message.indexOf(delimiter)).toInt();
      message.remove(0, message.indexOf(delimiter) + 1);
      int speed = message.toInt();

      if (action != NULL && power_ratio != NULL && speed != NULL) {
        update_motors(speed, action, power_ratio);
        Serial.println();
        Serial.println("After update:");
        Serial.print("  Left current:");
        Serial.println(current_left_speed);
        Serial.print("  Right current:");
        Serial.println(current_right_speed);
      }
    }
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
