// KOZAC Sensor
// Wireless sensor system.
// Uses Bluetooth to communicate.

#include "MilesTag2.h" //For receiving MilesTag 2 protocols



///////////////
// Prefix: SEN_
///////////////
// POWER
// Use 9V battery.
// Connect positive to VIN, negative to ground.
///////////////




int SEN_SensorPin; //Signal input pin
//Connected to the IR receiver.
int SEN_VibroPin; //Vibration
//Powers the vibration motor.
//////////////////////////////////
// Yes, you'll feel the hit     //
// while playing lasertag!!!    //
//////////////////////////////////
int SEN_SignalPin; //Signal output pin.

//For Bluetooth unit comm
int SEN_BT_RX;
int SEN_BT_TX;

////////////////////////////////
// POLIGON64 compatibility code
// P64 sensors have four terminals.
// V - power for the vibro and LED
// S - signal from the sensor
// Positive - power, connect to VIN
// Negative - ground, GND
////////////////////////////////
int SEN_P64_V; 
int SEN_P64_S;


void setup() {
  // put your setup code here, to run once:
  pinMode(SEN_SensorPin, INPUT);

  pinMode(SEN_VibroPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
