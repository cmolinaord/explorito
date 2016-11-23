#include <Wire.h>
#include <HMC5883L.h>

#include <Ultrasonic.h>
Ultrasonic ultrasonic(12,13);


int ledPin = 13; 

const float pi = 3.14;
float angulo_sensor = pi;

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
  Serial.println("Puerto serie escuchando");
  Wire.begin(); // Start the I2C interface.
  Serial.println("Started the I2C interface.");
  pinMode(ledPin, OUTPUT);   
  aviso_led();

  // Compass
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass to 1.3Ga
  if(error != 0){ // If there is an error, print it out. 
    Serial.println(compass.GetErrorText(error));
    error =0;
  }
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) {// If there is an error, print it out.
    Serial.println(compass.GetErrorText(error)); //Todo: Error handling for this method in .h and .cpp
    error=0;
  }

  Serial.println("Brujula configurada con exito");
  //Test azimut inicial
  /*
  float direccion = Azimut();
  Serial.print("Azimut inicial: ");
  Serial.print(direccion);
  Serial.println(" grados");
  */
  Serial.println("Finished void setup()");
  
}

void loop() {

  float distancia = sensor_ultra.Ranging(CM);
  float direccion = Azimut(); //degree
  float x = distancia * cos(direccion); //cm
  float y = distancia * sin(direccion); //cm
   
  Serial.print(direccion);
  Serial.print(" ");
  Serial.print(distancia);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.println(" ");
  
  aviso_led();
  delay(500);
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

