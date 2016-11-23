/*
 * Probando preguntas y respuestas a traves del Bluetooth
 */

//INCLUDES
//Bluetooth
#include <SoftwareSerial.h>  
#include <Wire.h>

#include <Ultrasonic.h>
Ultrasonic ultrasonic(12,13);

// Comunicacion serie
String inputCmd = "";
boolean stringComplete = false;

int ledPin = 13; 

const float pi = 3.14;
float angulo_sensor = pi;

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D4
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D5
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

// Ultrasonic
//Ultrasonic sensor_ultra(9,10);// (Trig PIN,Echo PIN)

// Compass
//HMC5883L compass;
//float declinationAngle = 0.22;
//int error = 0;
//int z = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait serial port. Leonardo only
  }
  inputCmd.reserve(20);

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
  /*compass = HMC5883L(); // Construct a new HMC5883 compass.
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

void loop(){


if (stringComplete){
  bluetooth.println(inputCmd);
  inputCmd = "reset";
  stringComplete=false;
}


}


//############################################
// Fuctions declaration
//############################################

// Serial Event //
// Registra eventos que entren al hardware por el pin RX
// Esta rutina corre entre cada iteracion de loop()
// Mejor no poner pausas en el loop() porque se puede perder info de comunicacion
void serialEvent(){
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    inputCmd += inChar;
    if (inChar == 'a') { // Si nueva linea FLAG
      stringComplete = true;
    }
  }
}


void aviso_led(){
  for(int i=1;i<4;i++){
  digitalWrite(ledPin,HIGH);delay(60);digitalWrite(ledPin,LOW);delay(60);
  }
}
/*
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
*/
