// Robo car // Luv_Sahu //

// Required Libraries
#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// Pin Definitions
#define TRIG_PIN A0 
#define ECHO_PIN A4 
#define MAX_DISTANCE 200 

#define MAX_SPEED 190  // DC motor max speed
#define TURN_SPEED 150 // Speed while turning

// Sensor and Motor Setup
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

// Global Variables
boolean goesForward = false;
int distance = 100;
int speedSet = 0;

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  
  myservo.attach(10);  
  myservo.write(115); 
  delay(1000);

  // Read initial distance
  distance = readPing();
  delay(100);
}

void loop() {
  int distanceR = 0, distanceL = 0;

  // Read distance & debug output
  distance = readPing();
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance <= 15) {  
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);

    // Look left & right for the best path
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    // Turn towards the clearer path
    if (distanceR >= distanceL) {
      turnRight();
    } else {
      turnLeft();
    }
  } else {  
    moveForward();
  }
}

int lookRight() {
    myservo.write(50); 
    delay(500);
    int dist = readPing();
    delay(100);
    myservo.write(115); // Reset servo position
    return dist;
}

int lookLeft() {
    myservo.write(170); 
    delay(500);
    int dist = readPing();
    delay(100);
    myservo.write(115); // Reset servo position
    return dist;
}

int readPing() { 
  delay(50);
  int cm = sonar.ping_cm();
  return (cm == 0) ? 250 : cm; // Avoid false 0 readings
}

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 

void moveForward() {
  if (!goesForward) {
    goesForward = true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(3);
    }
  }
}

void moveBackward() {
    goesForward = false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(3);
    }
}  

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  delay(500);
}

void turnLeft() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  delay(500);
}