/*
 * 3v1 es la version donde se implementa el manejo de Ardumoto
 * como en la version anterior pero añadiendo algunos sensores:
 *  Brujula (en el puerto I2C)
 *  Infrarrojos Sharp
 *  Ultrasonidos
 *  Comunicación Bluetooth
 * 31 - Jul - 2015
 */

/* Ardumoto Example Sketch
  by: Jim Lindblom
  date: November 8, 2013
  license: Public domain. Please use, reuse, and modify this 
  sketch!
  
  Three useful functions are defined:
    setupExplorito() -- Setup the Ardumoto Shield pins
    driveExplorito([motor], [direction], [speed]) -- Drive [motor] 
      (0 for A, 1 for B) in [direction] (0 or 1) at a [speed]
      between 0 and 255. It will spin until told to stop.
    stopExplorito([motor]) -- Stop driving [motor] (0 or 1).
  
  setupExplorito() is called in the setup().
  The loop() demonstrates use of the motor driving functions.
*/

// INCLUDES
#include <SoftwareSerial.h>  
#include <Wire.h>
#include <HMC5883L.h>
//#include <Ultrasonic.h>

// VARIALBLES GENERALES
const float pi = 3.14;
float angulo_sensor = pi;

// Bluetooth
SoftwareSerial bluetooth(4,5); // (TX,RX)

// Ultrasonic
//Ultrasonic sensor_ultra(9,10);// (Trig,Echo)

// Compass
HMC5883L compass;
float decAngle = -29.38E-3;
int error = 0;
int z = 0;

// Ardumotor
#define adelante  1
#define atras 0
#define MOTOR_IZQ 0  // B
#define MOTOR_DER 1  // A
const byte PWMA = 11;  // PWM control (speed) for motor IZQ  (B)
const byte PWMB = 10; // PWM control (speed) for motor DER (A)
const byte DIRB = 13; // Direction control for motor IZQ (B)
const byte DIRA = 12; // Direction control for motor DER (A)

void setup()
{
  Serial.begin(9600); 
  /*while (!Serial) {
    Serial.println("Serial port ready");
    }*/
  Wire.begin(); // Start the I2C interface.
  
  // Inicio Bluetooth
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600  
  while (!bluetooth) {
    Serial.println("Done bluetooth configuration!");
    bluetooth.println("Done bluetooth configuration!");
    }
  
  // Compass
  compass = HMC5883L(); // Construct a new HMC5883 compass. (I2C Address: 0x1E)
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

  
  float heading = azimut();
  bluetooth.print("Azimut inicial: ");
  bluetooth.print(heading);
  bluetooth.println(" grados");

  Serial.print("Azimut inicial: ");
  Serial.print(heading);
  Serial.println(" grados");


  
  setupExplorito(); // Set all pins as outputs
  stopExplorito();
  delay(200);
  //driveExplorito(MOTOR_DER, adelante, 210);
      driveExplorito(MOTOR_DER, adelante, 170);
      driveExplorito(MOTOR_IZQ, adelante, 178);
  
  delay(1500);
  stopExplorito();
  delay(500);
  bluetooth.println("He andado recto 2s");
  bluetooth.println("Mido mi azimut de nuevo...");
  float heading2 = azimut();
  bluetooth.print("Mi azimut es: ");
  bluetooth.print(heading2);
  bluetooth.println(" grados");
  if (abs(heading2-heading)>4) {
    bluetooth.println("Me he girado mas de 4 grados. Corrigo...");
  }
  while (abs(heading2-heading)>4) {
    if (heading2>heading) {
      // Tengo que girar a izq
      bluetooth.print(" < ");
      driveExplorito(MOTOR_DER, adelante, 220);delay(2);
      driveExplorito(MOTOR_IZQ, atras, 220);
      delay(100);
      stopExplorito();
      delay(1);
      heading2 = azimut();
      delay(1);
    }
    else {
      // Giro a la derecha
      bluetooth.print(" > ");
      driveExplorito(MOTOR_IZQ, adelante, 220);
      driveExplorito(MOTOR_DER, atras, 220);
      delay(100);
      stopExplorito();
      delay(1);
      heading2 = azimut();
      delay(1);
    }
    
  }
  delay(200);
  bluetooth.println();
  bluetooth.println("Direccion correcta:");
  float heading3 = azimut();
  bluetooth.print(heading3);
  bluetooth.println(" grados");
  
}

void loop()
{

}


//############################################
// Fuctions declaration

// driveExplorito() drives 'motor' in 'dir' direction at 'spd' speed
void driveExplorito(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_IZQ) // B
  {
    digitalWrite(DIRB, dir); bluetooth.print(" DIRB_izq ");bluetooth.print(dir);
    analogWrite(PWMB, spd); bluetooth.print(" PWMB_izq ");bluetooth.println(spd);bluetooth.println();
  }
  else if (motor == MOTOR_DER) // A
  {
    digitalWrite(DIRA, dir); bluetooth.print(" DIRA_der ");bluetooth.print(dir);
    analogWrite(PWMA, spd); bluetooth.print(" PWMA_der ");bluetooth.println(spd);bluetooth.println();
  }  
}

// stopExplorito() makes a motor stop
void stopExplorito()
{
  driveExplorito(MOTOR_DER, 0, 0);
  driveExplorito(MOTOR_IZQ, 0, 0);
}

// setupExplorito initialize all pins
void setupExplorito()
{
  // All pins should be setup as outputs:
  pinMode(PWMB, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  
  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

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

