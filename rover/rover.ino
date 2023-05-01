#include <AFMotor.h>
#include <Servo.h> 

// Drive Instructions
const String delimiter = ";";

// Motor Setup
AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);
Servo servo;
const int initial_servo_pos = 40;

#define RXpin 2
#define TXpin 3

// State trackers
int cut_state = 0;
int current_left_speed;
int current_right_speed;

void cut() {
  servo.write(5);
  delay(40);
  servo.write(75);
  delay(40);
  servo.write(40);
  delay(40);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting this up!");

  // Intialize the servo
  servo.attach(9);
  servo.write(40);

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

      Serial.println("Doing a drive:");
      Serial.println(action);
      Serial.println(power_ratio);
      Serial.println(speed);

      if (action != NULL && speed != NULL) {
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
}


