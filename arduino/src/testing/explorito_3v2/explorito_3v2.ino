#include <Adafruit_HMC5883_U.h>

#include <Arduino.h>

/*
 * 3v2 es la version mejorada de la version 3v1, en la que se le va a intentar 
 * añadir: 
 * - Un sistema de comandos con el que dar instrucciones a Explorito
 * Estos podrian ser alguno:
 *    - dir: estableze una direccion respecto a las coordenadas magneticas
 *    - av: avanza durante un cierto tiempo o distancia en la direccion actual
 *    - gd/gi: gira derecha/izquierda un cierto numero de grados.
 *    - mide: mide la distancia hasta el objeto mas cercano en frente suya
 *    - rodeo: mide la distancia a todos los puntos a sua alrededor mientras 
 *             realiza un giro de 360º
 * Mientras no se den ordenes Explorito debe quedarse quieto
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
char stringDER, stringIZQ, stringAV;

// VARIALBLES GENERALES
const float decAngle = -29.38E-3; // declinacion vector magnetico en Sevilla
const float tolAng = 2.0;  // Tolerancia angular para dar por buena una medida
float angulo_sensor = PI;

// Bluetooth
SoftwareSerial bluetooth(4,5); // (TX,RX)

// Ultrasonic
//Ultrasonic sensor_ultra(9,10);// (Trig,Echo)

// Compass
HMC5883L compass;

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

void setup()     // VOID SETUP ############################################
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
  bluetooth.println("Done compass configuration!");
  
  setupExplorito(); // Set all pins as outputs
  stopExplorito();
  bluetooth.println("Done motors configuration!");

  char stringDER = 'd';
  char stringIZQ = 'a';
  char stringAV = 'w';
}

void loop()  // VOID LOOP ############################################
{


if(bluetooth.available()>0){
  
  char lectura = bluetooth.read();
  bluetooth.println(lectura);
  Serial.println(lectura);
  /*
  if (lectura == stringDER) {
    gdExplorito(90,200);
  }
  else if (lectura == stringIZQ){
    giExplorito(90,200);
  }
  else if (lectura == stringAV) {
    avExplorito(1,200);
  }
  */
}


//driveExplorito(MOTOR_IZQ, adelante, 250);
//driveExplorito(MOTOR_DER, atras, 250);
/*
stopExplorito();
delay(400);

giExplorito(25,250);
delay(1000);
*/
}




//############################################
// Fuctions declaration

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

// driveExplorito() drives 'motor' in 'dir' direction at 'spd' speed
void driveExplorito(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_IZQ) // B
  {
    digitalWrite(DIRB, dir);// bluetooth.print(" DIRB_izq ");bluetooth.print(dir);
    analogWrite(PWMB, spd); //bluetooth.print(" PWMB_izq ");bluetooth.println(spd);bluetooth.println();
  }
  else if (motor == MOTOR_DER) // A
  {
    digitalWrite(DIRA, dir); // bluetooth.print(" DIRA_der ");bluetooth.print(dir);
    analogWrite(PWMA, spd); // bluetooth.print(" PWMA_der ");bluetooth.println(spd);bluetooth.println();
  }  
}

// stopExplorito() makes a motor stop
void stopExplorito()
{
  driveExplorito(MOTOR_DER, 0, 0);
  driveExplorito(MOTOR_IZQ, 0, 0);
}

void avExplorito(float tiempo, byte vel){
  driveExplorito(MOTOR_IZQ, adelante, vel);
  driveExplorito(MOTOR_DER, adelante, vel);
  delay(1000*tiempo);
  stopExplorito();
}

void gdExplorito(float angulo, int vel){  // GIRA DERECHA UN ANGULO DADO
  float dir0 = azimut(); float dir00 = dir0;
  float dir1 = corrige(dir0 + rad(angulo));
  // Lo pongo a girar y voy midiendo. Cuando esté alineado lo paro.
  driveExplorito(MOTOR_IZQ, adelante, vel);
  driveExplorito(MOTOR_DER, atras, vel);
  while (abs(dir0 - dir1) > rad(tolAng)){
      delay(20);
      dir0 =azimut();
      if ((dir0-dir1)>3* rad(tolAng)) { // Me he pasado (cambio de sentido)
        driveExplorito(MOTOR_IZQ, atras, vel);
        driveExplorito(MOTOR_DER, adelante, vel);
      }
      else if ((dir0-dir1)<3*rad(tolAng)) { // Si no he llegado o me he vuelto a pasar (giro a derechas)
        driveExplorito(MOTOR_IZQ, adelante, vel);
        driveExplorito(MOTOR_DER, atras, vel);
      }
  }
  stopExplorito(); // He salido del bucle
// Relata lo sucedido:
bluetooth.println("Giro a la derecha completado");
bluetooth.print("Angulo inicial: ");
bluetooth.println(grados(dir00));
bluetooth.print(grados(dir00));
bluetooth.print(" + ");
bluetooth.print(angulo);
bluetooth.print(" = ");
bluetooth.println(grados(dir1));
bluetooth.print("Mi direccion actual es: ");
bluetooth.println(grados(corrige(dir0)));
}

void giExplorito(float angulo, int vel){  // GIRA IZQUIERDA UN ANGULO DADO
  float dir0 = azimut(); float dir00 = dir0;
  float dir1 = corrige(dir0 - rad(angulo));
  // Lo pongo a girar y voy midiendo. Cuando esté alineado lo paro.
  driveExplorito(MOTOR_IZQ, atras, vel);
  driveExplorito(MOTOR_DER, adelante, vel);
  while (abs(dir0 - dir1) > rad(tolAng)){
      delay(20);
      dir0 =azimut();
      if ((dir0-dir1)<3*tolAng) { // Me he pasado (cambio de sentido)
        driveExplorito(MOTOR_IZQ, adelante, vel);
        driveExplorito(MOTOR_DER, atras, vel);
      }
      else if ((dir0-dir1)>3*tolAng) { // Si no he llegado o me he vuelto a pasar (giro a derechas)
        driveExplorito(MOTOR_IZQ, atras, vel);
        driveExplorito(MOTOR_DER, adelante, vel);
      }
  }
  stopExplorito(); // He salido del bucle
// Relata lo sucedido:
bluetooth.println("Giro a la izquierda completado");
bluetooth.print("Angulo inicial: ");
bluetooth.print(dir00);bluetooth.println("rad");
bluetooth.print(grados(dir00));bluetooth.println("grados");
bluetooth.print(grados(dir00));
bluetooth.print(" - ");
bluetooth.print(angulo);
bluetooth.print(" = ");
bluetooth.println(grados(dir1));
bluetooth.print("Mi direccion actual es: ");
bluetooth.println(grados(dir0));
}



float azimut(){ // Da el angulo hacia el que apunta la cabeza en radianes
  // Tiene en cuenta la orietacion del sensor con el cuerpo (angulo_sensor)
  //
  //  http://www.magnetic-declination.com/
  // San Juan de Aznalfarache: 1º40' = -29.38 mrad
  float decAngle = -29.38E-3;
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  heading = corrige(heading + decAngle - angulo_sensor);
  
  heading = heading * 180/PI;
  return heading;
}

float grados(float a){
  float b = a * 180/PI;
  return b;
}

float rad(float b){
  float a = b * PI /180;
  return a;
}

float corrige(float a){ // a es un numero en radianes
  // Si a esta fuera de rango, lo transforma para meterlo.
  
  if(a < 0)
     a += 2*PI;
  if(a > 2*PI)
     a -= 2*PI;
}

