/*
 * La version 2v0 es una mejora a la version 1v0 funcionando a día 11-Jul-2015 de madrugada
 * añadiendole dos funcionalidades basicamente:
 * - La comunicación se va a realizar por Bluetooth
 * - La comunicación va a ser bidireccional, esto es, podemos mandarle a Explorito que pare o empieze
 *   a tomar medidas ( o cuántas en concreto quiero tomar) desde la terminal de 'RealTerm'
 */

//INCLUDES
//Bluetooth
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <HMC5883L.h>

#include <Ultrasonic.h>
Ultrasonic ultrasonic(12,13);


int ledPin = 13;

const float pi = 3.14;
float angulo_sensor = pi;

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D4
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D5
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

// Ultrasonic
Ultrasonic sensor_ultra(9,10);// (Trig PIN,Echo PIN)

// Compass
HMC5883L compass;
float declinationAngle = 0.22;
int error = 0;
int z = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait serial port. Leonardo only
  }


  // Inicializa Bluetooth
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps

  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  while (!bluetooth) {
  ; // wait for bluetooth serial port to connect. Needed for Leonardo only
  }

  bluetooth.println("Puerto bluetooth transmitiendo");
  Serial.println("Puerto bluetooth transmitiendo");

  Wire.begin(); // Start the I2C interface.
  bluetooth.println("Started the I2C interface.");
  Serial.println("Started the I2C interface.");
  pinMode(ledPin, OUTPUT);  
  aviso_led();

  // Compass
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  bluetooth.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass to 1.3Ga
  if(error != 0){ // If there is an error, print it out.
    bluetooth.println(compass.GetErrorText(error));
    error =0;
  }
  bluetooth.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) {// If there is an error, print it out.
    bluetooth.println(compass.GetErrorText(error)); //Todo: Error handling for this method in .h and .cpp
    error=0;
  }

  bluetooth.println("Brujula configurada con exito");
  //Test azimut inicial
  /*
  float direccion = Azimut();
  Serial.print("Azimut inicial: ");
  Serial.print(direccion);
  Serial.println(" grados");
  */
  bluetooth.println("Finished void setup()");

}

void loop() {

  if (bluetooth.available()>0 {

  }

  float distancia = sensor_ultra.Ranging(CM);
  float direccion = Azimut(); //degree
  float x = distancia * cos(direccion); //cm
  float y = distancia * sin(direccion); //cm

  bluetooth.print(direccion);
  bluetooth.print(" ");
  bluetooth.print(distancia);
  bluetooth.print(" ");
  bluetooth.print(x);
  bluetooth.print(" ");
  bluetooth.print(y);
  bluetooth.println(" ");

  Serial.print(direccion);
  Serial.print(" ");
  Serial.print(distancia);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.println(" ");

  aviso_led();
  delay(50);



}


//############################################
// Fuctions declaration
//############################################
void aviso_led(){
  for(int i=1;i<4;i++){
  digitalWrite(ledPin,HIGH);delay(60);digitalWrite(ledPin,LOW);delay(60);
  }
}

//############################################
float Azimut(){
  // Calculate the azimut angle

  //  http://www.magnetic-declination.com/ --> San Juan de Aznalfarache: 1º40' = -29.38 mrad
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
