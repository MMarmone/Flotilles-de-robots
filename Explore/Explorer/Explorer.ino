#include <Adafruit_MotorShield.h>
#include <string.h>
#include "Memory.cpp"
#include "Pathfinding.cpp"
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(0,1);

#define FORWARD_ 0
#define BACKWARD_ 2
#define LEFT_ -1
#define RIGHT_ 1

/* Ultrasonic sensors */
const uint8_t echoPin_RIGHT = 2;  // echo signal (receives)
const uint8_t echoPin_right = 4;
const uint8_t echoPin_front_right = 6;
const uint8_t echoPin_front_left = 8;
const uint8_t echoPin_left = 10;
const uint8_t echoPin_LEFT = 12;

const uint8_t trigPin_RIGHT = 3;  // trigger signal (sends)
const uint8_t trigPin_right = 5;
const uint8_t trigPin_front_right = 7;
const uint8_t trigPin_front_left = 9;
const uint8_t trigPin_left = 11;
const uint8_t trigPin_LEFT = 13;


/* Measurements */
const float safetyDistance = 30; // according with the speed. Expressed in cm
const float robotWidth = 20; // expressed in cm
const float marge = safetyDistance / 3; // margin of movement. It should move between the marging and the safetyDistance


/* LEDs
   long side : pin
   short side : ground
   resistor : 100 Ohm
*/
const uint8_t ledPin_left = 15;
const uint8_t ledPin_back = 16;
const uint8_t ledPin_right = 17;


/*
   Determines where to move
*/
int objectDetected = 0; // the side where the object is detected
boolean searchingObject = false;
int tick = 0;
int randomDir = FORWARD_;
boolean stop_ = false;


/* Movement */
const int motorSpeed = 100; // from 0 (off) to 255 (max speed)

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Motor 1 -> right behind / Motor 2 -> right front / Motor 3 -> left front / Motor 4 -> left behind
Adafruit_DCMotor *motorRightBehind = AFMS.getMotor(3);
Adafruit_DCMotor *motorRightFront = AFMS.getMotor(4);
Adafruit_DCMotor *motorLeftFront = AFMS.getMotor(1);
Adafruit_DCMotor *motorLeftBehind = AFMS.getMotor(2);


class Explorer {

  public:
    /*********************************
       GENERAL SETTINGS
     *********************************/
    int SENSOR_NUMBER = 6;
    // Index of the positions
    int FAR_LEFT = 0, LEFT = 1, UP_LEFT = 2, UP_RIGHT = 3, RIGHT = 4, FAR_RIGHT = 5;
    // Angles of orientation of the sensors
    double angles[6] = { -90, -45, 20, -20, 45, 90};
    // Safety distance in mm
    int SAFETY_DISTANCE = 60;

    // Error margin when moving along an object (in mm)
    int ERROR_MARGIN = 30;
    // Distance at which we will move along an object (in mm)
    int FOLLOW_DISTANCE = 90;
    // Speed of the wheels
    int DEFAULT_SPEED = 100;

    /*********************************
       DEPENDS ON THE ROBOT'S SPEED
     *********************************/
    // Tick needed per angle
    double TICK_PER_ANGLE;
    double ANGLE_PER_TICK;
    // Tick per mm 
    double TICK_PER_MM;
    double MM_PER_TICK;
    /*********************************/



    /*********************************
       SPECIFIC TO THE ROBOT
     *********************************/
    // Position
    double x, y;
    double angle;

    // Distances from the robot to the closest objects
    double* distances;

    // If the robot will enter in contact with an object
    boolean* contact;

    // Previous informations
    double* prev_distances;
    boolean* prev_contact;

    // Which side do we follow ? (-1 is none)
    int FOLLOWING_SIDE = -1;
    int OLD_FOLLOWING_SIDE = FAR_RIGHT;
    Memory* memory;
    double total_distance;
    /*********************************/

    // Constructor
    Explorer(double TICK_PER_ANGLE, double TICK_PER_MM) {
      distances = new double[6];
      contact = new boolean[6];

      prev_distances = new double[6];
      prev_contact = new boolean[6];

      this->TICK_PER_ANGLE = TICK_PER_ANGLE;
      this->TICK_PER_MM = TICK_PER_MM;

      this->ANGLE_PER_TICK = 1 / TICK_PER_ANGLE;
      this->MM_PER_TICK = 1 / TICK_PER_MM;

      // Starting position
      x = 0; y = 0;
      angle = 0;
      total_distance = 0;

      FOLLOWING_SIDE = -1;
    }

    void setPos(int x, int y) {
      this->x = x;
      this->y = y;
    }

    void setAngle(int angle) {
      this->angle = angle;
    }

    double getX() {
      return x;
    }

    double getY() {
      return y;
    }

    double getAngle() {
      return angle;
    }

    double* getDistances() {
      return distances;
    }

    void setDistances(double* distances) {
      for (int i = 0; i < SENSOR_NUMBER; i++) this->distances[i] = distances[i];
    }

    /*
       Calculates the distance with the information obtained from the sensors
    */
    float calculDistance(uint8_t trigPin, uint8_t echoPin) {
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

    void updateDistance(int dir) {
      double dist = 0;
      switch (dir) {
        case 0:
          dist = calculDistance(trigPin_LEFT, echoPin_LEFT);
          break;
        case 1:
          dist = calculDistance(trigPin_left, echoPin_left);
          break;
        case 2:
          dist = calculDistance(trigPin_front_left, echoPin_front_left);
          break;
        case 3:
          dist = calculDistance(trigPin_front_right, echoPin_front_right);
          break;
        case 4:
          dist = calculDistance(trigPin_right, echoPin_right);
          break;
        case 5:
          dist = calculDistance(trigPin_RIGHT, echoPin_RIGHT);
          break;
      }
      distances[dir] = dist;
      contact[dir] = isThereSomething(dir, SAFETY_DISTANCE);
    }

    /**
       Update the distance using the ultrasounds
    */
    void updateDistance() {

      // Copy the previous informations
      for (int i = 0; i < SENSOR_NUMBER; i++) {
        prev_distances[i] = distances[i];
        prev_contact[i] = contact[i];
      }

      // Récupérer les informations des capteurs
      double cm_front_left = calculDistance(trigPin_front_left, echoPin_front_left);
      double cm_front_right = calculDistance(trigPin_front_right, echoPin_front_right);
      double cm_left = calculDistance(trigPin_left, echoPin_left);
      double cm_LEFT = calculDistance(trigPin_LEFT, echoPin_LEFT);
      double cm_right = calculDistance(trigPin_right, echoPin_right);
      double cm_RIGHT = calculDistance(trigPin_RIGHT, echoPin_RIGHT);

      //conversion cm -> mm
      distances[FAR_LEFT] = cm_LEFT * 10;
      distances[FAR_RIGHT] = cm_RIGHT * 10;
      distances[LEFT] = cm_left * 10;
      distances[RIGHT] = cm_right * 10;
      distances[UP_LEFT] = cm_front_left * 10;
      distances[UP_RIGHT] = cm_front_right * 10;

      for (int i = 0; i < SENSOR_NUMBER; i++) {
        if (distances[i] <= SAFETY_DISTANCE) contact[i] = true;
        else contact[i] = false;
      }
      String data = string_to_json();
      Serial.print(data);
      s.print(data);

    }

    void speedLeft(int motorSpeed) {
      motorLeftFront->setSpeed(motorSpeed);
      motorLeftBehind->setSpeed(motorSpeed);
    }

    void speedRight(int motorSpeed) {
      motorRightFront->setSpeed(motorSpeed);
      motorRightBehind->setSpeed(motorSpeed);
    }

    /**
       Move the robot
    */
    void explore() {
      updateDistance();

      if (FOLLOWING_SIDE == -1) find_();
      else if(!memory->isVisited(x, y, total_distance)) follow();
      else {
         // On a fini de visiter, on se tourne et on continue l'exploration
         for(int i = 0; i < 9; i++){
            if(FOLLOWING_SIDE == FAR_RIGHT) left(10);
            else right(10);
         }
         FOLLOWING_SIDE = -1;
      }
    }

    /**
       Move the robot "randomly" until finding an object to move along
    */
    void find_() {
      // If we're alongside an object on our left or right
      if (distances[OLD_FOLLOWING_SIDE] <= FOLLOW_DISTANCE + ERROR_MARGIN) {
         FOLLOWING_SIDE = OLD_FOLLOWING_SIDE;
         memory->eraseMemory();
         memory->setFollowingSide(FOLLOWING_SIDE);
      }
      else if (distances[FAR_LEFT] <= FOLLOW_DISTANCE + ERROR_MARGIN){
         FOLLOWING_SIDE = FAR_LEFT;
         memory->eraseMemory();
         memory->setFollowingSide(FOLLOWING_SIDE);
      }
      else if (distances[FAR_RIGHT] <= FOLLOW_DISTANCE + ERROR_MARGIN) {
         FOLLOWING_SIDE = FAR_RIGHT;
         memory->eraseMemory();
         memory->setFollowingSide(FOLLOWING_SIDE);
      }
      else {
        int cpt = 0;
        // Nothing ahead, go forward
        if (!contact[UP_RIGHT] && !contact[UP_LEFT] && !contact[RIGHT] && !contact[LEFT]) forward(30);
        // If there is something ahead, on the right, then we turn the robot to follow it
        else if (contact[UP_RIGHT] || contact[RIGHT]) {
          while (!(distances[FAR_RIGHT] <= FOLLOW_DISTANCE + ERROR_MARGIN) && cpt++ < 9) {
            left(10);
            updateDistance();
          }
        } else if (contact[UP_LEFT] || contact[LEFT]) {
          while (!(distances[FAR_LEFT] <= FOLLOW_DISTANCE + ERROR_MARGIN) && cpt++ < 9) {
            right(10);
            updateDistance();
          }
        }
      }
    }

    /**
       Move the robot to follow an object on a certain side
    */
    void follow() {
      // Si ça fait un moment qu'on a pas ajouté de points, alors on en ajoute un
      if(memory->distanceSinceLastPoint(total_distance) >= 200) memory->addMemoryPoint(x, y, total_distance);
       
      // On tourne de 20 degré quand y'a un truc devant
      if(contact[UP_RIGHT] || contact[UP_LEFT] || contact[RIGHT] || contact[LEFT]){
        if(FOLLOWING_SIDE == FAR_RIGHT) left(10);
        else right(10);
        updateDistance();
      }

      // If there is something on the side at a good distance, we continue to go forward
      if (isThereSomething(FOLLOWING_SIDE, FOLLOW_DISTANCE + ERROR_MARGIN + ERROR_MARGIN)) {
        // If we're too close, we try to turn a bit
        if (distances[FOLLOWING_SIDE] <= FOLLOW_DISTANCE - ERROR_MARGIN) {
          if (FOLLOWING_SIDE == FAR_RIGHT && distances[FOLLOWING_SIDE] < prev_distances[FOLLOWING_SIDE]) left(10);
          if (FOLLOWING_SIDE == FAR_LEFT && distances[FOLLOWING_SIDE] < prev_distances[FOLLOWING_SIDE]) right(10);
          updateDistance();
        } else if (distances[FOLLOWING_SIDE] >= FOLLOW_DISTANCE + ERROR_MARGIN) {
          if (FOLLOWING_SIDE == FAR_RIGHT && distances[FOLLOWING_SIDE] > prev_distances[FOLLOWING_SIDE]) right(10);
          if (FOLLOWING_SIDE == FAR_LEFT && distances[FOLLOWING_SIDE] > prev_distances[FOLLOWING_SIDE]) left(10);
          updateDistance();
        }
        if (!contact[UP_RIGHT] && !contact[UP_LEFT] && !contact[LEFT] && !contact[RIGHT]) forward(30);
        else backward(30);
      }
      else {
        // If there is no more something at the side, we try to turn and go forward
        int cpt = 0;
        while (!isThereSomething(FOLLOWING_SIDE, FOLLOW_DISTANCE + ERROR_MARGIN) && cpt < 12) {
          if(contact[UP_RIGHT] || contact[UP_LEFT] || contact[LEFT] || contact[RIGHT] || contact[FAR_RIGHT] || contact[FAR_LEFT]) break;
          if (FOLLOWING_SIDE == FAR_RIGHT) right(10);
          else if (FOLLOWING_SIDE == FAR_LEFT) left(10);
          forward(30);
          updateDistance();
          cpt++;
        }

        // Still nothing ? We stop to follow and try to explore something else
        if (!isThereSomething(FOLLOWING_SIDE, FOLLOW_DISTANCE + ERROR_MARGIN)) {
          OLD_FOLLOWING_SIDE = FOLLOWING_SIDE;
          FOLLOWING_SIDE = -1;
        }
      }
    }

    boolean isThereSomething(int dir, int distance) {
      return distances[dir] <= distance;
    }

    // Direction
    void forward() {
      // Wheels go forward
      motorLeftFront->run(BACKWARD);
      motorRightFront->run(FORWARD);
      motorLeftBehind->run(BACKWARD);
      motorRightBehind->run(FORWARD);
    }
    void left() {
      // Wheels go left
      motorLeftFront->run(FORWARD);
      motorRightFront->run(FORWARD);
      motorLeftBehind->run(FORWARD);
      motorRightBehind->run(FORWARD);
    }
    void right() {
      // Wheels go right
      motorLeftFront->run(BACKWARD);
      motorRightFront->run(BACKWARD);
      motorLeftBehind->run(BACKWARD);
      motorRightBehind->run(BACKWARD);
    }
    void backward() {
      // Wheels go backward
      motorLeftFront->run(FORWARD);
      motorRightFront->run(BACKWARD);
      motorLeftBehind->run(FORWARD);
      motorRightBehind->run(BACKWARD);
    }

    void stopmamene() {
      motorLeftFront->run(RELEASE);
      motorLeftBehind->run(RELEASE);
      motorRightFront->run(RELEASE);
      motorRightBehind->run(RELEASE);
      delay(500);
      //for (int i = 0; i < 100; i++) {}
    }
    /**
       Update the position of the robot given the distance travelled
       @param distance
    */
    void updatePos(double distance) {
      total_distance += distance;
      double angle = 3.141592654 * this->angle / 180;
      x += cos(angle) * distance;
      y += sin(angle) * distance;
    }

    /**
       Move forward for the specified distance (in mm)
       @param distance
    */
    void forward(int distance) {
      for (int i = 0; i < distance * TICK_PER_MM; i++) {
        forward();
      }
      updatePos(distance);
      stopmamene();
    }

    /**
       Move left for the specified angle (in degree)
       @param angle
    */
    void left(int angle) {
      for (int i = 0; i < angle * TICK_PER_ANGLE; i++) {
        left();
      }
      addAngle(-angle);
      stopmamene();
    }

    /**
       Move right for the specified angle (in degree)
       @param angle
    */
    void right(int angle) {
      for (int i = 0; i < angle * TICK_PER_ANGLE; i++) {
        right();
      }
      addAngle(angle);
      stopmamene();
    }

    /**
       Move backward for the specified distance (in mm)
       @param distance
    */
    void backward(int distance) {
      for (int i = 0; i < distance * TICK_PER_MM; i++) {
        backward();
      }
      updatePos(-distance);
      stopmamene();
    }

    /**
       Add the @param angle to the general angle. It will ensure that the general angle belong to [0, 360[ (in degree)
       @param angle
    */
    void addAngle(double angle) {
      this->angle += angle;
      if (this->angle >= 360) this->angle -= 360;
      if (this->angle < 0) this->angle += 360;
    }

    //traduit x,y,angle,distances en JSON
    String string_to_json(){
      String res = "{ ";
      res += "\"x\" : ";
      res += String(x);
      res += ", \"y\" : ";
      res += String(y);
      res += ", \"angle\" : ";
      res += String(angle);
      res += ", \"distances\" : { \"0\" : ";
      res += String(distances[0]);
      res += ", \"1\" : ";
      res += String(distances[1]);
      res += ", \"2\" : ";
      res += String(distances[2]);
      res += ", \"3\" : ";
      res += String(distances[3]);
      res += ", \"4\" : ";
      res += String(distances[4]);
      res += ", \"5\" : ";
      res += String(distances[5]);
      res += " }";
      res += " };";
      return res;
    }

};

Explorer* robot;
boolean test;

void setup() {
  Serial.begin(115200);
  AFMS.begin();

  robot = new Explorer(1.80, 0.62);

// Left wheels
  robot->speedLeft(motorSpeed*1.1);

  // Right wheels
  robot->speedRight(motorSpeed);

  robot->stopmamene();

  delay(1000); //on attends 5 sec la connexion au serveur
  //robot->left(90);
  /*int i;
  for(i=0;i < 10; i++){
    //robot->left(10);
    robot->forward(30);
  }
  //robot->forward(10);*/
}

void loop() {
  robot->explore();
  /*Serial.print("Reading data...");
  String data = Serial.readStringUntil(';');
  Serial.println("Data: " + data);*/
  
}
