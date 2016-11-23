//5-Jul-2015
//
// Probando a transmitir por bluetooth datos recogidos por tres sensores a la vez:
//   - Distancia por infrarojos: Sharp
//   - Distancia por ultrasonidos: HC-SR04
//   - Magnetometro: HMC5883L

//        Arduino |   Device
// Bluetooth  ----------------     
//           D4   |   TX-O
//           D5   |   RX-I
//           5V   |   VCC
// Sharp IR    ----------------     
//           5V   |   Rojo
//           GND  |   Negro
//           A0   |   Amarillo
// Compass HMC5883L ----------------     
//         3.3V   |   VCC
//           D4   |   SDA
//           D5   |   SCL
// Ultrasonidos ----------------     
//         5V     |   VCC
//          D9    |   Trig
//          D10   |   Echo

//INCLUDES
//Bluetooth
#include <SoftwareSerial.h>  
#include <Wire.h>

#include <HMC5883L.h>
//#include <Ultrasonic.h>

//General
//int ledPin = 13; 
// Bluetooth
SoftwareSerial bluetooth(4, 5);

// Ultrasonic
//Ultrasonic ultrasonic(9,10);// (Trig PIN,Echo PIN)

// Compass
HMC5883L compass;
float declinationAngle = 0.22;
int error = 0;
int z = 0;

void setup() {
  // Initialize
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  Wire.begin(); // Start the I2C interface.
  //pinMode(ledPin, OUTPUT);


  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600  

  
  delay(500);

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
  Serial.println("Finished void setup()");

  delay(500);
  float heading = azimut();
  bluetooth.print("Azimut inicial: ");
  bluetooth.print(heading);
  bluetooth.println(" grados");
  
  delay(2000);
  /*  for(int i=1;i<4;i++){
  digitalWrite(ledPin,HIGH);delay(40);digitalWrite(ledPin,LOW);delay(60);
  }*/
  
}  

void loop() {
  /*
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());  
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
  }
  // and loop forever and ever!
  */

bluetooth.print("Hola ordenador");

  float heading = azimut();
  bluetooth.print("Azimut inicial: ");
  bluetooth.print(heading);
  bluetooth.println(" grados");
  
  delay(1000);
}



//############################################
// Fuctions declaration
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




