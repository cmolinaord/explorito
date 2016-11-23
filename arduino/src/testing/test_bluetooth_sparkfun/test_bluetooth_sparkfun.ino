/*
  Example Bluetooth Serial Passthrough Sketch
 by: Jim Lindblom
 SparkFun Electronics
 date: February 26, 2013
 license: Public domain

 This example sketch converts an RN-42 bluetooth module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and bluetooth module.
 */
#include <SoftwareSerial.h>  

int bluetoothTx = 4;  // TX-O pin of bluetooth mate, Arduino D4
int bluetoothRx = 5;  // RX-I pin of bluetooth mate, Arduino D5

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600

  delay(200);
  bluetooth.print("\nbluetooth.available() = ");
  bluetooth.println(bluetooth.available());
  /*
  while (!bluetooth.available()) {
    bluetooth.print(".");
  }*/

  
  bluetooth.print("\nSerial.available() = ");
  bluetooth.println(Serial.available());
  
  while (!Serial.available()) {
    bluetooth.print(".");
  }  
  Serial.print("\nSerial.available() = ");
  Serial.println(Serial.available());
  Serial.print("\nbluetooth.available() = ");
  Serial.println(bluetooth.available());    
  
  Serial.print("\nbluetooth.isListening() = ");
  Serial.println(bluetooth.isListening());   
  Serial.print("\nbluetooth.isListening() = ");
  Serial.println(bluetooth.isListening());   
  bluetooth.listen();
  Serial.print("\nbluetooth.isListening() = ");
  Serial.println(bluetooth.isListening());   
  
}

/*
 * A día Lunes 10 agosto de madrugada funciona solo la comunicacion
 * desde el puerto serie a que se imprima por puerto bluetooth.
 * La comunicacion desde lectura por bluetooth a salida en Serial no funciona
 * Me da siempre que bluetooth.available() = 0  y que bluetooth.isListening() = 0
 */


void loop()
{
  
  if(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print(bluetooth.read());  
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
  }
  // and loop forever and ever!
}

