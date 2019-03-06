#pragma once
#include <Adafruit_MotorShield.h>
#define FORWARD_ 0
#define BACKWARD_ 2
#define LEFT_ -1
#define RIGHT_ 1
#define Robot_h

class Robot {

private:
	int objectDetected = 0; // the side where the object is detected
	int tick = 0;
	const float safetyDistance = 10; // according with the speed. Expressed in cm
	const float robotWidth = 20; // expressed in cm
	const float marge = safetyDistance; // margin of movement. It should move between the marging and the safetyDistance
	Adafruit_MotorShield AFMS = Adafruit_MotorShield();// Create the motor shield object with the default I2C address
	Adafruit_DCMotor *motorLeft = AFMS.getMotor(4);// Motor 4 -> left 
	Adafruit_DCMotor *motorRight = AFMS.getMotor(2);// Motor 2 -> right
	volatile int currentState = FORWARD_;
	boolean stop_ = false;
	const int motorSpeed = 200;

public:
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
	Robot();
	void setupMotors();
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void dontMove();
	float calculDistance(uint8_t trigPin, uint8_t echoPin);
	int isTheirNothingAround(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT);
	int TheirSomethingOnLeft(float cm_left, float cm_LEFT);

	int TheirSomethingOnRight(float cm_right, float cm_RIGHT);

	int TheirSomethingOnLEFT(float cm_LEFT);

	int TheirSomethingOnRIGHT(float cm_RIGHT);
	int TheirSomethingOnFront(float cm_front_left, float cm_front_right, float cm_left, float cm_right);
	int IsTheirEnouthSpaceToTurn(float cm_front_left, float cm_front_right);

	int explore(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT);

	void navigate();
};