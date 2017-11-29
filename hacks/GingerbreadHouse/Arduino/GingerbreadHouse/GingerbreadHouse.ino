/* SMART GINGERBREAD HOUSE
 *  
 * Have fun =)
 *
 * This file is part of the Estefannie Explains It All repo.
 *
 * (c) Estefannie Explains It All <estefannieexplainsitall@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Servo.h>
#include "IRremote.h"

//Wiring
const int doorServoPin = 3;
const int gingerbreadMenServosPin = 9;
const int receiver = 11;

int length = 26;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int beats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};
int tempo = 200;
int song = 0;
int lightsBlink = 0;
int dance = 0;
int doorOpen = 0;

int spin = 0;

int incomingByte = 0;   // for incoming serial data

// create a servo object to control servos
Servo doorServo; 
Servo gingerbreadMenServos;
int servoPosition;
int pos = 0;    // variable to store the servo position
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

int musicPlaying = 0;

void(* resetFunc) (void) = 0;

void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); resetFunc(); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: { //TURN MUSIC ON AND OFF
    Serial.println("1"); 
    if (musicPlaying == 0){
      musicPlaying = 1;}
    else { musicPlaying = 0;}}
    break;
  
  case 0xFF18E7: {//TURN LIGHTS ON AND OFF
    Serial.println("2");
    if (lightsBlink == 0){
      lightsBlink = 1;}
    else { lightsBlink = 0;}}   
   break;
  case 0xFF7A85:{ //TURN DANCE ON AND OFF
    Serial.println("3");
    if (dance == 0){
      dance = 1;}
    else { dance = 0;}}   break;  
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button   ");

  }// End Case

  delay(500); // Do not get immediate repeat
}
void playTone(int tone, int duration) {

  if (!gingerbreadMenServos.attached()){gingerbreadMenServos.attach(gingerbreadMenServosPin);}
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(13, HIGH);
    digitalWrite(6, HIGH);
    delayMicroseconds(tone);
    digitalWrite(13, LOW);
    digitalWrite(6, LOW);
    pos + 15;
    gingerbreadMenServos.write(pos);
    delayMicroseconds(tone);
    if (pos > 360){pos = 0;}
    }
  
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        Serial.println("IR Receiver Button Decode"); 
    irrecv.enableIRIn(); // Start the receiver
    pinMode(13, OUTPUT);
       
        pinMode(6, OUTPUT);

        servoPosition = 0;
        doorServo.attach(doorServoPin);
        gingerbreadMenServos.attach(gingerbreadMenServosPin);
}

int doorState = 0;
int danceState = 0;
int lightState = 0;

long prev_door = 0;
long prev_dance = 0;
long prev_light = 0;

long door_dt = 0;
long dance_dt = 0;
long light_dt = 0;

void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
  
  unsigned long dt = millis();
  
        if (musicPlaying == 1)
        {
          if (notes[song] == ' ') 
          {
              delay(beats[song] * tempo); // rest
          }
          else 
          {
              playNote(notes[song], beats[song] * tempo);
          }
          // pause between notes
          delay(tempo / 2);

          song++;

          if (song >= length)
            song = 0;
        }

        if (lightsBlink == 1 && musicPlaying == 0) {
          if (lightState == 0) {
           digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
            if ((dt - light_dt) - prev_light > 1000)
            {
              prev_light = (dt - light_dt);
              lightState = 1;
            }
          }
          if (lightState == 1) {
           digitalWrite(6, LOW);   // turn the LED on (HIGH is the voltage level)
           if ((dt - light_dt) - prev_light > 1000)
           {
            prev_light = (dt - light_dt);
            lightState = 0;
           }
          }
          
        }
        else if (lightsBlink == 0) {digitalWrite(6,LOW);}
        
        if (dance == 1 && musicPlaying == 0) {

          if (gingerbreadMenServos.attached()){
            for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
         // in steps of 1 degree
       gingerbreadMenServos.write(pos);              // tell servo to go to position in variable 'pos'
         delay(15);                      // waits 15ms for the servo to reach the position
         }
         for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
          gingerbreadMenServos.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                     // waits 15ms for the servo to reach the position
            }
          
          }
          else {gingerbreadMenServos.attach(gingerbreadMenServosPin);}
        }
        else if (dance == 0) {
          gingerbreadMenServos.detach();
        }
        
      
        if (doorOpen == 1) {
          if (doorState == 0)
          {
              doorServo.write(90);
              if ((dt - door_dt) - prev_door > 8 * 90)
              {
                prev_door = (dt - door_dt);
                doorState = 1;
                Serial.print(doorState);
              }
          }
          if (doorState == 1)
          {
              doorServo.write(270);
              if ((dt - door_dt) - prev_door > 8 * 180)
              {
                prev_door = (dt - door_dt);
                doorState = 2;
                Serial.print(doorState);
              }
          }
          if (doorState == 2)
          {
              doorServo.write(180);
              doorState = 3;//done
              Serial.print(doorState);
          }
        }
        else if (doorOpen == 0) {
          if (doorState == 0)
          {
              doorServo.write(180);
              if ((dt - door_dt) - prev_door > 8 * 90)
              {
                prev_door = (dt - door_dt);
                doorState = 1;
                Serial.print(doorState);
              }
          }
          if (doorState == 1)
          {
              doorServo.write(270);
              if ((dt - door_dt) - prev_door > 8 * 180)
              {
                prev_door = (dt - door_dt);
                doorState = 2;
                Serial.print(doorState);
              }
          }
          if (doorState == 2)
          {
              doorServo.write(90);
              doorState = 3;//done
              Serial.print(doorState);
          }
        }

}
 
