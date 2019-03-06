#include "RobotHeader.h"
#include <stdio.h>
#include <Arduino.h>

Robot::Robot() {
	// initialize serial communication:
	Serial.begin(9600);
	AFMS.begin();  // create with the default frequency 1.6KHz
	setupMotors();
}

/*
	* Motors setup and movement
	*/
void Robot::setupMotors() {
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
	motorLeft->run(RELEASE);
	motorRight->run(FORWARD);
}

void Robot::moveRight() {
	motorLeft->run(FORWARD);
	motorRight->run(RELEASE);
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
int Robot::isTheirNothingAround(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT) {
	return cm_LEFT > (safetyDistance + marge) && cm_left > (safetyDistance + marge) && cm_front_left > (safetyDistance + marge) && cm_front_right > (safetyDistance + marge) && cm_right > (safetyDistance + marge) && cm_RIGHT > (safetyDistance + marge);
}
int Robot::TheirSomethingOnLeft(float cm_left, float cm_LEFT) {
	return cm_LEFT < safetyDistance + marge || cm_left < safetyDistance + marge;;
}

int Robot::TheirSomethingOnRight(float cm_right, float cm_RIGHT) {
	return cm_RIGHT < safetyDistance + marge || cm_right < safetyDistance + marge;
}

int Robot::TheirSomethingOnLEFT(float cm_LEFT) {
	return cm_LEFT < safetyDistance + marge;
}

int Robot::TheirSomethingOnRIGHT(float cm_RIGHT) {
	return cm_RIGHT < safetyDistance + marge;
}
int Robot::TheirSomethingOnFront(float cm_front_left, float cm_front_right, float cm_left, float cm_right) {
	return cm_front_right < safetyDistance + marge || cm_front_left < safetyDistance + marge || cm_right < safetyDistance + marge || cm_left < safetyDistance + marge;
}
int Robot::IsTheirEnouthSpaceToTurn(float cm_front_left, float cm_front_right) {
	return cm_front_left > safetyDistance || cm_front_right > safetyDistance;
}

int Robot::explore(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT) {
	return FORWARD_;
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
