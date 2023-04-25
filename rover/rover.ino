// https://lastminuteengineers.com/l293d-motor-driver-shield-arduino-tutorial/
// #include <AFMotor.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

// Drive Instructions
const char *CMD_FORWARD = "forward";
const char *CMD_BACKWARD = "backward";
const char *CMD_STOP = "stop";
const int TERM = '.';
const String delimiter = ";";

// [f || b][l || r]_motor
// AF_DCMotor fl_motor(1);
// AF_DCMotor fr_motor(2);
// AF_DCMotor bl_motor(3);
// AF_DCMotor br_motor(4);
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *fl_motor = AFMS.getMotor(1);
Adafruit_DCMotor *fr_motor = AFMS.getMotor(2);
Adafruit_DCMotor *bl_motor = AFMS.getMotor(3);
Adafruit_DCMotor *br_motor = AFMS.getMotor(4);

TwoWire myWire = Wire();  // use Wire1 for communication

#define RXpin 2
#define TXpin 3
SoftwareSerial Printer(RXpin, TXpin);

char *buffer;

void setup() {
  Serial.begin(9600);
  Printer.begin(115200);
  Serial.println("Starting this up!");

  // Connect to motor driver shield
  if (!AFMS.begin(0x60, &myWire)) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  //Set initial speed of the motor & stop
	fl_motor->setSpeed(0);
	fl_motor->run(RELEASE);

  fr_motor->setSpeed(0);
  fr_motor->run(RELEASE);

  bl_motor->setSpeed(0);
  bl_motor->run(RELEASE);

  br_motor->setSpeed(0);
  br_motor->run(RELEASE);
}

// int calculate_left_speed(speed int, ratio int) {
//   return 
// }

void loop() {
  if (Serial.peek() != -1) {
    String message = Serial.readStringUntil('\n');
    // Parse command aspects
    int action = message.substring(0, message.indexOf(delimiter)).toInt();
    message.remove(0, message.indexOf(delimiter) + 1);
    int power_ratio = message.substring(0, message.indexOf(delimiter)).toInt();
    message.remove(0, message.indexOf(delimiter) + 1);
    int speed = message.toInt();

    if (action != NULL && power_ratio != NULL && speed != NULL) {
      if (action == 1) {
        // Move forward
        Serial.println("moving forward!");
        fl_motor->run(FORWARD);
        fl_motor->setSpeed(speed);

        fr_motor->run(FORWARD);
        fr_motor->setSpeed(speed);

        bl_motor->run(BACKWARD);
        bl_motor->setSpeed(speed);

        br_motor->run(BACKWARD);
        br_motor->setSpeed(speed);
      } else if (action == 2) {
        // Move backwards
        Serial.println("moving backward!");
        fl_motor->run(BACKWARD);
        fl_motor->setSpeed(speed);

        fr_motor->run(BACKWARD);
        fr_motor->setSpeed(speed);
        
        bl_motor->run(FORWARD);
        bl_motor->setSpeed(speed);

        br_motor->run(FORWARD);
        br_motor->setSpeed(speed);
      } else if (action == 0) {
        // Stop
        Serial.println("stopping!");
        fl_motor->run(RELEASE);
        fr_motor->run(RELEASE);
        bl_motor->run(RELEASE);
        br_motor->run(RELEASE);
      }
    }
  }
  // delay(1500);
}


