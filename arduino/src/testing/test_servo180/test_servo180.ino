
/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep

 modified 4 Sep 2015
 by Carlos Molina
*/

#include <Servo.h>
#include <Wire.h>

Servo myservo;  // create servo object to control a servo
                // twelve servo objects can be created on most boards

int vel_0 = 90;    // variable to store the servo position
int vel_pos = 98;
int vel_neg = 80;

int pos = 0;

void setup()
{

  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 (PWM) to the servo object


for(int i = 0; i <= 10; i++){
  myservo.write(40);
  delay(100);
  myservo.write(30);
  delay(100);
}
}

void loop()
{
  for(int i = 0; i <= 250; i++){
pos = myservo.read();// tell servo to go to position in variable 'pos'
myservo.write(i);
Serial.println(pos);
if(i == 0){
  delay(1000);
}
else
{
  delay(30);
}
}
}
