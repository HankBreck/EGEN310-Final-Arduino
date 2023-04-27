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
  
	m1.setSpeed(200);
	m1.run(RELEASE);

  m2.setSpeed(200);
  m2.run(RELEASE);

  m3.setSpeed(200);
  m3.run(RELEASE);

  m4.setSpeed(200);
  m4.run(RELEASE);

  current_left_speed = 0;
  current_right_speed = 0;
}

void get_speeds(int speed, int ratio, int& left_speed, int& right_speed) {
    if(ratio == 50) {
        left_speed = speed;
        right_speed = speed;
    } else if(ratio < 50) {
        left_speed = speed * (100 - ratio) / 50;
        right_speed = speed;
    } else {
        left_speed = speed;
        right_speed = speed * ratio / 50;
    }
}

void update_speed(int current, int target) {
  if (current < target) {
    for (int i=current; i<target; i++) {
      m1.setSpeed(i);
      m2.setSpeed(i);
      m3.setSpeed(i);
      m4.setSpeed(i);
      current_speed = i;
    }
  } else if (current > target) {
    for (int i=current; i>target; i--) {
      m1.setSpeed(i);
      m2.setSpeed(i);
      m3.setSpeed(i);
      m4.setSpeed(i);
      current_speed = i;
    }
  }
}

void update_motors(int speed, int direction, int ratio) {
  int left_speed, right_speed;
  get_speeds(speed, ratio, &left_speed, &right_speed);
  if (direction == FORWARD) {
    Serial.println("moving forward!");
    m1.run(FORWARD);
    m2.run(FORWARD);
    m3.run(BACKWARD);
    m4.run(BACKWARD);
  } else if (direction == BACKWARD) {
    Serial.println("moving backward!");
    m1.run(BACKWARD);
    m2.run(BACKWARD);
    m3.run(FORWARD);
    m4.run(FORWARD);
  } else {
    Serial.println("releasing!");
    m1.run(RELEASE);
    m2.run(RELEASE);
    m3.run(RELEASE);
    m4.run(RELEASE);
  }

  update_speeds(current_speed, speed);
  return;
}

void loop() {
  if (Serial.peek() != -1) {

    // Parse command
    String message = Serial.readStringUntil('\n');
    int action = message.substring(0, message.indexOf(delimiter)).toInt();
    message.remove(0, message.indexOf(delimiter) + 1);

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
        update_motors(speed, action);
      }
    }
  }
}


