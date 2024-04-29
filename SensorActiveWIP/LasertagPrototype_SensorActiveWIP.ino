// KOZAC Active Sensor
// Wireless/Wired sensor system.
// Uses Bluetooth to communicate.

#include "IRremote.hpp"



///////////////
// Prefix: SEN_
///////////////
// POWER
// Use 9V battery.
// Connect positive to VIN, negative to ground.
///////////////
/* Pinout:
 Bluetooth/Serial
 0 - RXD (Serial in)
 1 - TXD (Serial out)
 Sensor signal lines (IR)
 2 - S1 (Sensor 1 (front) signal in)
 3 - S2 (Sensor 2 (back) signal in)
 4 - S3 (Sensor 3 (left) signal in)
 5 - S4 (Sensor 4 (right) signal in)
 Vibration motor feedback
 6 - V1 (Sensor vibration 1 (front) feedback)
 7 - V2 (Sensor vibration 2 (back) feedback)
 8 - V3 (Sensor vibration 3 (left) feedback)
 9 - V4 (Sensor vibration 4 (right) feedback)
 RGB LED hit indication
 10 - R (RGB LED)
 11 - G (RGB LED)
 12 - B (RGB LED)
 13 - Reserved
*/




////////////
// Sensor pins
//Each Connected to the corresponding IR receiver.
IRrecv SEN_S1(2); //Front
IRrecv SEN_S2(3); //Back
IRrecv SEN_S3(4); //Left
IRrecv SEN_S4(5); //Right
decode_results results;
//////////////////////////////////
// Vibration pins               //
// Powers the vibration motor.  //
//////////////////////////////////
// Yes, you'll feel the hit     //
// while playing lasertag!!!    //
//////////////////////////////////
int SEN_V1=6; //Front
int SEN_V2=7; //Back
int SEN_V3=8; //Left
int SEN_V4=9; //Right



//For Bluetooth unit comm
int SEN_BT_RX=0;
int SEN_BT_TX=1;


void setup() {
  // put your setup code here, to run once:
  pinMode(SEN_V1,OUTPUT);
  pinMode(SEN_V2,OUTPUT);
  pinMode(SEN_V3,OUTPUT);
  pinMode(SEN_V4,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
