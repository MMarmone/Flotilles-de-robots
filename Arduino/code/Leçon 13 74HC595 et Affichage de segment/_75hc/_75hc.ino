//www.elegoo.com
//2016.06.13

// Lab13 Using a 74hc595 and seven segment display to make reciprocal function
 
// define the LED digit patterns, from 0 - 9
// 1 = LED on, 0 = LED off, in this order:
//                74HC595 pin     Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7 
//                Mapping to      a,b,c,d,e,f,g of Seven-Segment LED
byte seven_seg_digits[10] = { B11111100,  // = 0
                              B01100000,  // = 1
                              B11011010,  // = 2
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11100110   // = 9
                             };
 
// connect to the ST_CP of 74HC595 (pin 12,latch pin)
int latchPin = 8;
// connect to the SH_CP of 74HC595 (pin 11, clock pin)
int clockPin = 12;
// connect to the DS of 74HC595 (pin 14)
int dataPin = 11;
 
void setup() {
  // Set latchPin, clockPin, dataPin as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Enter a Number between 0 to 9");
}
 
// display a number on the digital segment display
void sevenSegWrite(byte digit) {
  // set the latchPin to low potential, before sending data
  digitalWrite(latchPin, LOW);
     
  // the original data (bit pattern)
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
 
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);
}
 
void loop() {       
  // count from 9 to 0
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch >= '0' && ch <= '9')
    {
      
      sevenSegWrite(0);
      int led = ch - '0';
      sevenSegWrite(led);
      Serial.print("Turned on Number ");      
      Serial.println(led);
      delay(500);
    }
    
  }
}
