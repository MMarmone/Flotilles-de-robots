/****************************************************************************
   Sonar Robot

   Authors:  
      - BENSOUSSAN ChloÃ©
      - BOUKOU GrÃ¢ce
      - GRÃ‰AU Alexandre      
      - WILHELM Andreina
          
   Permissions: MIT licence
      
*****************************************************************************/

#include <Adafruit_MotorShield.h>



#define FORWARD_ 0
#define BACKWARD_ 2
#define LEFT_ -1
#define RIGHT_ 1

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


/* Measurements */

/* LEDs
 * long side : pin
 * short side : ground
 * resistor : 100 Ohm
 */
const uint8_t ledPin_left = 15;
const uint8_t ledPin_back = 16;
const uint8_t ledPin_right = 17;


/*
 * Determines where to move
 */



/* Movement */
 // from 0 (off) to 255 (max speed)








/*
 * LEDs setup
 */ 
void setupLeds() {
  pinMode(ledPin_left, OUTPUT);
  pinMode(ledPin_back, OUTPUT);
  pinMode(ledPin_right, OUTPUT);  
}

void turnOffAllLeds() { 
  digitalWrite(ledPin_left, LOW);
  digitalWrite(ledPin_back, LOW);
  digitalWrite(ledPin_right, LOW);
  delay(100);
}

void turnOnLed(uint8_t ledPin) {
  digitalWrite(ledPin, HIGH);  
}



/*
 * Calculates the distance with the information obtained from the sensors  
 */
float calculDistance(uint8_t trigPin, uint8_t echoPin){
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
  cm = (float)((duration<<4)+duration)/1000.0; // cm = 17 * duration/1000
  return cm;
}


void initValue() {
  objectDetected = 0;
  searchingObject = true;
  tick = 0;
}

/*
 * Determines where to move
 */
int isTheirNothingAround(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT){
  return cm_LEFT > (safetyDistance + marge) && cm_left> (safetyDistance + marge) && cm_front_left > (safetyDistance + marge) && cm_front_right > (safetyDistance + marge) && cm_right> (safetyDistance + marge) && cm_RIGHT > (safetyDistance + marge);
}
int TheirSomethingOnLeft(float cm_left, float cm_LEFT){
  return cm_LEFT < safetyDistance + marge || cm_left < safetyDistance + marge;;
}

int TheirSomethingOnRight(float cm_right, float cm_RIGHT){
  return cm_RIGHT < safetyDistance + marge || cm_right < safetyDistance + marge;
}

int TheirSomethingOnLEFT(float cm_LEFT){
  return cm_LEFT < safetyDistance + marge;
}

int TheirSomethingOnRIGHT(float cm_RIGHT){
  return cm_RIGHT < safetyDistance + marge;
}
int TheirSomethingOnFront(float cm_front_left, float cm_front_right,float cm_left,float cm_right){
  return cm_front_right < safetyDistance + marge || cm_front_left < safetyDistance + marge || cm_right < safetyDistance + marge || cm_left < safetyDistance + marge;
}
int IsTheirEnouthSpaceToTurn(float cm_front_left, float cm_front_right){
  return cm_front_left > safetyDistance || cm_front_right > safetyDistance  ;
}

int explore(float cm_LEFT, float cm_left, float cm_front_left, float cm_front_right, float cm_right, float cm_RIGHT) {  
  
  
  //si on cherche un objet
  if(searchingObject == true ){
    Serial.println("_On Cherche un objet :");
    //si il n'y a rien au tour alors on va tout droit
    if(isTheirNothingAround(cm_LEFT, cm_left, cm_front_left, cm_front_right, cm_right, cm_RIGHT)){
       Serial.println("/t -Il n'y a rien autour !!");
      return FORWARD_;
    }else{
      //si il y a quelque chose en face alors 
      Serial.println("/t -Il y a quelque chose autour :");
      if(TheirSomethingOnFront(cm_front_left, cm_front_right, cm_left, cm_right)){
        Serial.println("/t/t -Il y a quelque chose  en face");
        //si il y a suffisament de palce pour tourner
        if(IsTheirEnouthSpaceToTurn(cm_front_left, cm_front_right)){
          // si il y a plus de place a gauche que a droite
          if (cm_left > cm_right || cm_LEFT > cm_RIGHT) {
            Serial.println("/t/t/t -on va a gauche");
            //alors on tourne a gauche
            objectDetected = 1; // l'objet est a droite
            searchingObject = false; // est n'est plus en recherche d'objet
            return LEFT_; 
          }
          else {
             Serial.println("/t/t/t -on va a droite");
            //sinon on tourne a gauche
            objectDetected = 2; // l'objet est a gauche
            searchingObject = false;
            return RIGHT_;
          }  
        }
        else {
          Serial.println("/t/t/t -On fait demi tour car il n'y a pas assez de place pour tourner");
          return BACKWARD_;
        }
      }
      if(TheirSomethingOnLEFT(cm_LEFT)){
        Serial.println("/t/t -Il y a quelque chose a gauche alors on va tout droit");
        objectDetected = 2; // l'objet est a gauche
        searchingObject = false;
        return FORWARD_;
      }
      if(TheirSomethingOnRIGHT(cm_RIGHT)){        
        Serial.println("/t/t -Il y a quelque chose a droite alors on va tout droit");
        objectDetected = 1; // l'objet est a droite
        searchingObject = false;
        return FORWARD_;
        
      }
    }
    Serial.println("/t -on va tout droit parce qu'on sait pas quoi faire");
    return FORWARD_;
  }else{
    //si l'object est a droite
    if(objectDetected == 1){
      Serial.println("_On a trouvé un objet a droite :");
      
      //si il y a quelque chose en face
      if(TheirSomethingOnFront(cm_front_left, cm_front_right, cm_left, cm_right)){
        Serial.println("\t -il y a quelque chose en face");
        //alors on doit tourner a gauche si il y a assez de place
        if(IsTheirEnouthSpaceToTurn(cm_front_left, cm_front_right)){
          return LEFT_;
        }
        //sinon on recule
        else {
          Serial.println("\t\t -On a perdu l'objet on fait marche arrière");
          objectDetected = 0; // l'objet n'existe pas
          searchingObject = true;// on cherche un objet
          return BACKWARD_;
        }
      }
      //on verifie si l'objet n'est pas la
      if(!TheirSomethingOnRIGHT(cm_RIGHT)){        
        Serial.println("\t -On a perdu l'objet a droite on tourne pour le retrouver ");
        if(saved_tick == 0){
          saved_tick = tick;
        }else{
          if(tick < saved_tick+15){
            return RIGHT_;
          }else{
             objectDetected = 0; // l'objet n'existe pas
             searchingObject = true;// on cherche un objet
             saved_tick = 0;
          }
        }
       
        //alors on tourne a droite
        return RIGHT_;
      }
      //si il n'y a rien devant on continue a suivre l'object courant
      if(TheirSomethingOnRIGHT(cm_RIGHT)){   
        Serial.println("\t -il y a quelque chose sur la droite on continue d'avancer");
        return FORWARD_; 
      }
      return FORWARD_; 
    }
    if(objectDetected == 2){
       Serial.println("_On a trouvé un objet a gauche :");
      
      if(TheirSomethingOnFront(cm_front_left, cm_front_right, cm_left, cm_right)){
        Serial.println("\t -il y a quelque chose en face");
        if(IsTheirEnouthSpaceToTurn(cm_front_left, cm_front_right)){
          return RIGHT_;
        }
        else {
          Serial.println("\t\t -On a perdu l'objet on fait marche arrière");
          objectDetected = 0; // l'objet n'existe pas
          searchingObject = true;// on cherche un objet
          return BACKWARD_;
        }
      }
      if(!TheirSomethingOnLEFT(cm_LEFT)){        
        Serial.println("\t -On a perdu l'objet a gauche on tourne pour le retrouver ");
         if(saved_tick == 0){
          saved_tick = tick;
        }else{
          if(tick < saved_tick+15){
            return RIGHT_;
          }else{
             objectDetected = 0; // l'objet n'existe pas
             searchingObject = true;// on cherche un objet
             saved_tick = 0;
          }
        }
        return LEFT_;
        
      }
      if(TheirSomethingOnLEFT(cm_LEFT)){   
         Serial.println("\t -il y a quelque chose sur la gauche on continue d'avancer");
        return FORWARD_; 
      }
      return FORWARD_; 
    }
    return FORWARD_;
  }
} 


/*
 * Moves the wheels
 */
void navigate(){
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

  if (stop_){
    dontMove();
    return;
  }
  
  // turn off leds
  // turnOffAllLeds();
  currentState = resultatExplore;
  motorRight->run(RELEASE);
  motorLeft->run(RELEASE);

  switch(resultatExplore) {
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


/*
 * Initial setup
 */
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  setupMotors();
  //setupLeds();
}


/*
 * It's the function that will be called at each tick time
 */
void loop()
{
  navigate();
}
