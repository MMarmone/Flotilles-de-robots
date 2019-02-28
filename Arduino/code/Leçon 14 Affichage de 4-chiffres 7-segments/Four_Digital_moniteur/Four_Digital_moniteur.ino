int latch=9;  //74HC595  pin 12 STCP
int clock=10; //74HC595  pin 11 SHCP
int data=8;   //74HC595  pin 14 DS

void setup() {
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Hello");
}
void Display(unsigned char num)
{

  digitalWrite(latch,LOW);
  digitalWrite(latch,HIGH);
  
}
void loop() 
{
  while (Serial.available()) // tant que des caractères sont en attente
  {
    char ch = Serial.read(); // on lis le caratère  
    int c = ch - '0';
    Serial.print(ch); // on l'affiche
    Serial.print('\t'); // on ajoute une tabulation
    Serial.print(int(ch)); //on affiche le code correspondant     
    Serial.print('\t'); // on ajoute une tabulation
    Serial.println(char(int(ch)));

    Serial.println(c); // on affiche c
    
    digitalWrite(latch,c);
    delay(500);
    
    
  }
}
