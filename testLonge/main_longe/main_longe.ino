/****************************************************************************
   Sonar Robot

   Authors:  
      - Malalel
      - Yung
      - Valdenaire
      - Marmone--Marini 
      
          
   Permissions: MIT licence
      
*****************************************************************************/

#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#define FORWARD_ 0
#define BACKWARD_ 2
#define LEFT_ -1
#define RIGHT_ 1
#define Robot_h
#include <RadioHead.h>
/************************************************************************************/
/************************************************************************************/

class Robot {

public:

  int objectDetected = 0; // the side where the object is detected
  int tick = 0;
  int saved_tick = 0;
  const float safetyDistance = 10; // according with the speed. Expressed in cm
  const float safetyDistanceLR = 5;
  const float safetyDistanceFrontLR = 9; 
  const float robotWidth = 20; // expressed in cm
  const float marge = 2*safetyDistance; // margin of movement. It should move between the marging and the safetyDistance

  Adafruit_MotorShield AFMS = Adafruit_MotorShield();// Create the motor shield object with the default I2C address
  Adafruit_DCMotor *motorLeft = AFMS.getMotor(4);// Motor 4 -> left 
  Adafruit_DCMotor *motorRight = AFMS.getMotor(2);// Motor 2 -> right

  volatile int currentState = FORWARD_;
  boolean stop_ = false;
  const int motorSpeed = 100;
  
  /* Ultrasonic sensors */
  const uint8_t echoPin_RIGHT = 12;  // echo signal (receives)
  const uint8_t echoPin_right = 10;
  const uint8_t echoPin_front_right = 8;
  const uint8_t echoPin_front_left = 6;
  const uint8_t echoPin_left = 4;
  const uint8_t echoPin_LEFT = 2;

  const uint8_t trigPin_RIGHT = 13;  // trigger signal (sends)
  const uint8_t trigPin_right = 11;
  const uint8_t trigPin_front_right = 9;
  const uint8_t trigPin_front_left = 7;
  const uint8_t trigPin_left = 5;
  const uint8_t trigPin_LEFT = 3;
  
  Robot::Robot() {
    AFMS.begin();  // create with the default frequency 1.6KHz
    // initialize serial communication:
    setupMotors();
  }
  Robot::~Robot(){};
  /*
    * Motors setup and movement
    */
  void Robot::setupMotors() {
    Serial.println("on a bien setup les moteur");
    // Left wheel
    motorLeft->setSpeed(motorSpeed);
    motorLeft->run(FORWARD);
    motorLeft->run(RELEASE);
    // Right wheel
    motorRight->setSpeed(motorSpeed);
    motorRight->run(FORWARD);
    motorRight->run(RELEASE);
  }
  
  void Robot::moveForward() {
    motorRight->run(FORWARD);
    motorLeft->run(FORWARD);
  }
  
  void Robot::moveBackward() {
    motorRight->run(BACKWARD);
    motorLeft->run(BACKWARD);
  }
  
  void Robot::moveLeft() {
    motorLeft->run(BACKWARD);
    motorRight->run(FORWARD);
  }
  
  void Robot::moveRight() {
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
  }
  
  void Robot::dontMove() {
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
  }
  float Robot::calculDistance(uint8_t trigPin, uint8_t echoPin) {
    uint32_t duration; // duration of the round trip
    float cm;  // distance of the obstacle
  
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(3);
  
    // Start trigger signal
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
  
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
  
    // convert the time into a distance
    cm = (float)((duration << 4) + duration) / 1000.0; // cm = 17 * duration/1000
    return cm;
  }
  int Robot::isThereNothingAround(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT) {
    return cm_LEFT > (safetyDistance + marge) && cm_left > (safetyDistance + marge) && cm_front_left > (safetyDistance + marge) && cm_front_right > (safetyDistance + marge) && cm_right > (safetyDistance + marge) && cm_RIGHT > (safetyDistance + marge);
  }
  int Robot::ThereSomethingOnLeft(float cm_left, float cm_LEFT) {
    return cm_LEFT < safetyDistanceLR || cm_left < safetyDistance;
  }
  
  int Robot::ThereSomethingOnRight(float cm_right, float cm_RIGHT) {
    return cm_RIGHT < safetyDistanceLR || cm_right < safetyDistance;
  }
  
  int Robot::ThereSomethingOnLEFT(float cm_LEFT) {
    return cm_LEFT < safetyDistanceLR + marge;
  }
  
  int Robot::ThereSomethingOnRIGHT(float cm_RIGHT) {
    return cm_RIGHT < safetyDistanceLR;
  }
  int Robot::ThereSomethingOnFront(float cm_front_left, float cm_front_right, float cm_left, float cm_right) {
    return cm_front_right < safetyDistance || cm_front_left < safetyDistance || cm_right < safetyDistanceFrontLR  || cm_left < safetyDistanceFrontLR ;
  }
  int Robot::IsThereEnouthSpaceToTurn(float cm_front_left, float cm_front_right) {
    return cm_front_left > safetyDistance || cm_front_right > safetyDistance;
  }
  
  int Robot::explore(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT) {
    if(objectDetected == 0){
      if(isThereNothingAround(cm_LEFT, cm_left, cm_front_left, cm_front_right, cm_right, cm_RIGHT)){
        
        return FORWARD_;
      }
      if(ThereSomethingOnLEFT(cm_LEFT)){
        objectDetected = 1;
        return FORWARD_;
      }
      if(ThereSomethingOnFront(cm_front_left, cm_front_right, cm_left, cm_right)){
         
        objectDetected = 1;
        return RIGHT_;   
      }
       return FORWARD_;
    }else{
      
      if(ThereSomethingOnFront(cm_front_left, cm_front_right, cm_left, cm_right)){
         
        objectDetected = 1;
        return RIGHT_;   
      }
      if(cm_LEFT< 30 &&cm_LEFT >10){
        objectDetected = 0;
        return LEFT_;
      }
      if(!ThereSomethingOnLEFT(cm_LEFT)){
        if(saved_tick == 0){
          saved_tick = tick;
          return LEFT_;
        }else{
          if(tick < saved_tick+15){
              return LEFT_;
          }else{
            objectDetected = 0; // l'objet n'existe pas
            saved_tick = 0;
          }
        }
      return FORWARD_;
     }
     objectDetected = 0;
     return FORWARD_;
  }
}
  
  void Robot::navigate() {
    int resultatExplore;
  
    float cm_front_left;  // distance of the obstacle
    float cm_front_right;
    float cm_left;
    float cm_LEFT;
    float cm_right;
    float cm_RIGHT;
  
  
    noInterrupts();
    cm_front_left = calculDistance(trigPin_front_left, echoPin_front_left);
    cm_front_right = calculDistance(trigPin_front_right, echoPin_front_right);
    cm_left = calculDistance(trigPin_left, echoPin_left);
    cm_LEFT = calculDistance(trigPin_LEFT, echoPin_LEFT);
    cm_right = calculDistance(trigPin_right, echoPin_right);
    cm_RIGHT = calculDistance(trigPin_RIGHT, echoPin_RIGHT);
  
    resultatExplore = explore(cm_LEFT, cm_left, cm_front_left, cm_front_right, cm_right, cm_RIGHT);
    interrupts();
  
    if ((currentState == RIGHT_ || currentState == LEFT_) && (resultatExplore == LEFT_ || resultatExplore == RIGHT_)) {
      resultatExplore = currentState;
    }
    tick++;
  
    if (stop_) {
      dontMove();
      return;
    }
  
    // turn off leds
    // turnOffAllLeds();
    currentState = resultatExplore;
    motorRight->run(RELEASE);
    motorLeft->run(RELEASE);
  
    switch (resultatExplore) {
      // move forward  
    case FORWARD_:
      //Serial.print("avant  ");
      moveForward();
      break;
  
      // move backward
    case BACKWARD_:
      //Serial.print("arriere  ");
      moveBackward();
      break;
  
      // move left
    case LEFT_:
      //Serial.print("gauche  ");
      moveLeft();
      break;
  
      // move right
    case RIGHT_:
      //Serial.print("droite  ");
      moveRight();
      break;
    }
  }
};


/************************************************************************************/
//Main//
/************************************************************************************/




/*
 * Initial setup
 */
Robot *robot;
void setup() {
  Serial.begin(9600);
  robot = new Robot();
  //setupLeds();
}

/*
 * It's the function that will be called at each tick time
 */
void loop()
{
  robot->navigate();
}
