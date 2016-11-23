#include "Ultrasonic.h"
#include <Wire.h>
#include <HMC5883L.h>
#include <Servo.h>

//General
int ledPin = 13; 


HMC5883L compass;
float declinationAngle = 0.22;
int error = 0;















void setup() {

  // Initialize
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Wire.begin(); // Start the I2C interface.


  // Compass
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  error = compass.SetScale(1.3); // Set the scale of the compass to 1.3Ga
  if(error != 0){ // If there is an error, print it out. 
    Serial.println(compass.GetErrorText(error));
    error =0;
  }
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) {// If there is an error, print it out.
    Serial.println(compass.GetErrorText(error)); //Todo: Error handling for this method in .h and .cpp
    error=0;
  }
  Serial.println("Done compass configuration!");

  int z = 0;
  float heading = azimut();
  Serial.print("Initial orientation is: ");
  Serial.println(heading);
  

}

void loop() {

     float heading = azimut();
     Serial.print(heading);
     Serial.print(" ==> ");
     //delay(500);
     int z = zona(heading,0,5);
     Serial.println(z);
     delay(100);
  
}

// Fuction declaration
float azimut(){
  // Calculate the azimut angle
  
  //  http://www.magnetic-declination.com/
  // San Juan de Aznalfarache: 1º40' = -29.38 mrad
  float decAngle = -29.38E-3;
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  heading += decAngle;
  if(heading < 0)
     heading += 2*PI;
  if(heading > 2*PI)
     heading -= 2*PI;
  heading = heading * 180/PI;
  return heading;
}

int zona(float angle,float objetivo,float margin){
  // Calculate in which zone is located the given "angle"
  // using the reference "objetivo" angle with a margin of +- "margin"
  
  // Output Z (Zone)
  //   Zone A: 1
  //   Zone 0: 0
  //   Zone B: -1 
  
  float diff = objetivo - angle;
  int Z=0;
  if(diff > -180 && diff < -margin || diff > 180 && diff < 360+margin){
    Z = 1;  // Zone A
    }
  else if (diff > margin && diff < 180 || diff > -360-margin && diff < -180){
    Z = -1; // Zone B
    }
  else{
    Z = 0; //Zone 0
    }
  return Z;
}

