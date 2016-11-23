#include "Ultrasonic.h"
Ultrasonic ultrasonic(12,13);// (Trig PIN,Echo PIN)

void setup() {
delay(1000);

}

void loop()
{

  float tiempo = ultrasonic.Timing();
  Serial.print(tiempo);
  Serial.print(" us  ");
  Serial.print(" ==> ");
  Serial.print(0.017*tiempo);
  Serial.println(" cm");
    
  delay(50);
}




