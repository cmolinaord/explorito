/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
#include <Wire.h>
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int vel_0 = 90;    // variable to store the servo position 
int vel_pos = 98;
int vel_neg = 80;
 
void setup() 
{ 
  
  Serial.begin(9600);
  myservo.attach(6);  // attaches the servo on pin 7 to the servo object 
  

myservo.writeMicroseconds(1500);
  delay(1000);
myservo.writeMicroseconds(1600);
  delay(1000);
myservo.writeMicroseconds(1300);
  delay(1000);
myservo.writeMicroseconds(1900);
  delay(1000);
myservo.writeMicroseconds(1050);
  delay(1000);
} 
 
void loop() 
{ 
  for(int i=80; i<110; i++){
    myservo.write(i);              // tell servo to go to position in variable 'pos' 
    delay(100);    // waits 15ms for the servo to reach the position 
    
}

} 

