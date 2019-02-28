//www.elegoo.com
//2016.06.13

#include "pitches.h"
 
// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
 
void setup() {
 
}
 
void loop() {  
    // pin8 output the voice, every scale is 0.5 sencond
    tone(8, melody[3], 100);
    delay(100);
    tone(8, melody[3], 100);
    delay(500);
    tone(8, melody[3], 100);
    delay(400);
    tone(8, melody[1], 100);
    delay(100);
    tone(8, melody[3], 100);
    delay(400);
    tone(8, melody[5], 100);
    delay(500);
    tone(8, melody[5], 100);
    delay(500);
    tone(8, melody[1], 100);
    delay(500);
    tone(8, melody[5], 100);
    delay(500);
    tone(8, melody[3], 100);
    delay(500);
    tone(8, melody[6], 100);
    delay(500);
    tone(8, melody[7], 100);
    delay(500);
    tone(8, melody[6], 100);
    delay(500);
}
