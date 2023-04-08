/////////////////////////////////
// KOZAC LASERTAG SYSTEM       //
// PROTOTYPE FOR FORCE UA CLUB //
// KiEV 2023                   //
// SLAVA UKRAiNi! PUTiN HUiLO! //
/////////////////////////////////
// Rev. 0.01 Kurchatko         //
/////////////////////////////////

#include "IRremote.h"


////////////////////////////////////////////////////
// PREFIXES
// GUN - Related to the gun
// EXT - External sensors
// SYS - System
// LW - LASERWAR
// P64 - Poligon64
// LSD - LSD Electronics
////////////////////////////////////////////////////
// POWER
// Power pin goes into Arduino VIN pin.
// Uin = 7-12 V
// We will use either 9V or airsoft gun batteries.
// 9V has 600 mAh, airsoft gun batteries run from
// 800 to 5000 mAh.
////////////////////////////////////////////////////
// CHARGING
// Connect 2.5x5.5 mm charging jack positive center pin
// to battery positive. Negative to the ground.
////////////////////////////////////////////////////
// LOGIC
// The gun trigger, reload and sensor logic pins are
// powered by the +5V output from the Arduino.
// Connect them via the pull-up resistor.
////////////////////////////////////////////////////
// PROTOCOL
// By default, MilesTag 2 protocol would be used.
////////////////////////////////////////////////////

//////////////////
// Gun magazine //
//////////////////
#include "Magazine.h"
///////////////////////
// MilesTag 2 SYSTEM //
///////////////////////
#include "MilesTag2.h"

//Pins
int GUN_TriggerPin; //Trigger pin.
bool GUN_TriggerPressed; //Is gun trigger pressed?


int GUN_ReloadPin; //Reload pin.
int GUN_SelectorPin; //Selector pin
//For burst/full auto fire.

int GUN_MagazinePin; //Magazine pin

int GUN_Speaker; //Speaker output pin
int GUN_Vibro; //Vibro blowback pin

//Output pins for IR and LED signal.
int GUN_IR; //IR LED
int GUN_LED; //LED
///////////////////////////////////
// GUN_LED as a shot indicator is set to steady on.
// GUN_IR is pulsed according to MilesTag 2 or other protocol.


// Compatibility
// LASERWAR emitter has 3 pins. A common positive power, the LED negative and IR negative.
// The output Arduino pins are connected to the bases of the pull-down transistors that
// control the IR and LEDs.
// Connect the each negative to the collector of each transistor, and ground the emitters.
////////////////////////////////////////////////////////
// LSD and Poligon64 emitters have separate positives and are to be controlled
// by directly connecting to the Arduino output pins.




//////////////////////////////////////////
// IRremote.h uses pin 3 for IR output. //
//////////////////////////////////////////


int GUN_Sensor; //Sensor pin for built in sensor in the gun
// 4 zone sensors
// Front - 1, Rear - 2, Left - 3, Right - 4
int EXT_Sensor1; //External sensor pin. 
int EXT_Sensor2;
int EXT_Sensor3;
int EXT_Sensor4;
int EXT_Vibro; // Hit output for the sensors. Fires a vibro and LED.



///////////////////////////////
// Gun Configuration
///////////////////////////////
// Magazines
// By default, 4 magazines with 30 rounds
int GUN_MagazineCount; //How many magazines you have
int GUN_MagazineCapacity; //Capacity of each magazine
//TODO: Use various capacity magazines
//TODO: Use detachable magazines for realism

GUN_Magazine GUN_Magazines[100]; //Max. 100 mags

//Gun damage
int GUN_Damage;



//Fire rate
int GUN_FireRate; //Fire rate in RPM. Dictates how much time it should fire per minute.
// Use this calculation:
// delay((1/(GUN_FireRate/60))/1000)
// or
// delay((60/GUN_FireRate)/1000)
// or
// delay(60/GUN_FireRate*1000)
// or
// delay(0.06/GUN_FireRate)
// after calling GUN_Fire()

//Gun fire mode. For selective fire weapon.
int GUN_FireMode; //0 - semi, 1 - auto, 2 - burst
const int GUN_FireMode_SEMI = 0;
const int GUN_FireMode_AUTO = 1;
const int GUN_FireMode_BURST = 2;

int GUN_BurstCount; //How much rounds it will burst

//Player parameters
int HP;

////////////
int MyTeamID;
int MyPlayerID;

//Arduino pinout compatibility.
int SYS_CompatibilityMode; //0 - no compatibility, 1 - LASERWAR, 2 - LSD, 3 - Poligon64.
//Future protocol reserved value.
int SYS_Protocol = 1; //1 for MilesTag 2

//Pins for Russian laser tag manufacturers compatibility
/////////////////////////
//       LASERWAR      //
/////////////////////////
// LASERWAR has three pin emitters. It has common positive and two output pins for LED and IR.
// LW emitters are controlled by the two pull-down NPN transistors.
// Connect common positive to +5V, IR and LED negative pins to the collector of each transistors, and ground the emitters.
// Connect GUN_IR and GUN_LED pins to the base of each transistor.
// NPN transistor works like a switch.
// Current goes from collector to emitter. If the base is powered, it switches on.
///////////////////////////////////
int LW_EmitterOUT; // Connect this pin to the base of a control pull down transistor
int LW_LEDOUT;     // Connect this pin to the base of a control pull down transistor




int LW_Bluetooth_MODE;
int LW_Bluetooth_RESET;
int LW_Bluetooth_STATUS;
int LW_Bluetooth_RX; //Bluetooth RX pin
int LW_Bluetooth_TX; //Bluetooth TX pin
//LASERWAR Bluetooth module pins

///////////////////////////////////////
// LASERWAR Built-In Receiver (TSOP) //
// 1 - OUT                           //
// 2 - GND                           //
// 3 - Vss                           //
///////////////////////////////////////
int LW_TSOP_1;
int LW_TSOP_2;
int LW_TSOP_3;
////////////////////////////////////////
// LSD and Poligon have direct connection to the output.
///////////////////////////////////////////


int P64_IROUT; //Poligon64 IR positive
int P64_LEDOUT; //Poligon64 LED positive
//Poligon64 has three pin emitters. It has two separate positives for LED and IR and one common ground.

int LSD_IROUT; //LSD IR positive
int LSD_LEDOUT; //LSD LED positive
//LSD has four pin emitters. LED and IR emitter have separate circuits. All negatives are separate, but here they go to the ground.

void setup() {
  // put your setup code here, to run once:



  ///////////////////////////////////////////
  // Set up the pin numbers of each pin
  //////////////////////////
  // IR pin is pin 3 since it sends PWM signal
  GUN_IR=3;
  // LED pin has to send the steady signal for the fire flash
  // Non-PWM pin would be used for this purpose.
  GUN_LED=4;

  //Gun trigger setup
  pinMode(GUN_TriggerPin, INPUT);
  //Reload setup
  pinMode(GUN_ReloadPin, INPUT);
  pinMode(GUN_SelectorPin, INPUT);

  //Sensors
  pinMode(GUN_Sensor, INPUT);
  IRrecv GUN_IRrecv_Sensor(GUN_Sensor); //IRremote System code 
  
  pinMode(EXT_Sensor1, INPUT);
  IRrecv GUN_IRrecv_Sensor1(EXT_Sensor1); //IRremote System code




  //Control the 
  pinMode(GUN_IR, OUTPUT);
  pinMode(GUN_LED, OUTPUT);


  ////////////////////////
  // COMPATIBILITY CODE //
  ////////////////////////
  // LASERWAR  //
  ////////////////////////
  // LW has 3 pin 
  // +5V, LED out and IR out
  pinMode(LW_EmitterOUT, OUTPUT);
  pinMode(LW_LEDOUT, OUTPUT);

  
  // Poligon64 //
  pinMode(P64_IROUT, OUTPUT); // Poligon64 IR emitter out
  pinMode(P64_LEDOUT, OUTPUT); //Poligon64 LED out
  //    LSD    //
  pinMode(LSD_IROUT, OUTPUT); //LSD IR out
  pinMode(LSD_LEDOUT, OUTPUT); //LSD LED out
  /////////////////////////
  switch(SYS_CompatibilityMode)
  {
    case 1: //LASERWAR
    {
      //Since LW IR and LED pins are output, they are connected to the bases of each transistor.
      //WARNING: DO NOT CONNECT LASERWAR EMITTERS DIRECTLY TO THE OUTPUT PINS!!!
      LW_EmitterOUT=GUN_IR;
      LW_LEDOUT=GUN_LED; 
      
      
    }
    case 2: //LSD
    {
        LSD_IROUT=GUN_IR;
        LSD_LEDOUT=GUN_LED;
    }
    case 3: //Poligon64
    {
            P64_IROUT=GUN_IR;
            P64_LEDOUT=GUN_LED;
    }
    default:
    {
      
    }
  }

  //Set up gun mag arrays
  //GUN_Magazine GUN_Magazines[GUN_MagazineCount](GUN_MagazineCapacity); //To be reinitialized
  for(int i=0;i<GUN_MagazineCount;i++)
  {
    GUN_Magazines[i]=GUN_Magazine(GUN_MagazineCapacity);
  }

  
}




//Gun trigger operation
void GUN_Trigger()
{

  //Trigger logic. Returns when the trigger is pressed.
  GUN_TriggerPressed=digitalRead(GUN_TriggerPin);
  if(GUN_TriggerPressed==HIGH)
  {
    //Check ammo
    if(GUN_Magazines[0].ammo>0)
    {
      //Check firemode
      //If full auto
      if (GUN_FireMode == GUN_FireMode_AUTO)
      {
        //Fire until the mag is empty
        while(GUN_TriggerPressed)
        {
          GUN_Fire();
          delay((1/(GUN_FireRate/60))/1000);
        }


      }
      //If burst
      else if (GUN_FireMode == GUN_FireMode_BURST)
      {
        //Count the burst
        for(int i=0;i<GUN_BurstCount;i++)
        {
          GUN_Fire();
          delay((1/(GUN_FireRate/60))/1000);
        }

      }
      //If semi
      else
      {
        //Fire just once.
        GUN_Fire();
        delay((1/(GUN_FireRate/60))/1000);

      }
    }
    else
    {
          //Play the "click" sound
    }
  }

}
/////////////////
// Gun fire
////////////////
// MilesTag 2 shot
// [Header]-[0ppppppp]-[ttdddd]
// p - Player ID BIN
// DEC: 0-127
// t - Team ID BIN
// DEC: 0-3
// d - Damage BIN
// DEC: 0-16
/////////////////////
void GUN_Fire()
{
  //Is the magazine empty?
  if(GUN_Magazines[0].ammo>0)
  {
    GUN_Magazines[0].ammo-=1; //Round away

    //Fire the gun by sending an IR pulse.
    MT2_Fire(GUN_IR, MyPlayerID, MyTeamID, GUN_Damage);
    //Play gunshot sound
    
  }
  else //Out of ammo
  {
    //Do not fire
    //Do not send the IR pulse
    digitalWrite(GUN_IR, LOW); //All zero
  }
}

//Gun reload
void GUN_Reload()
{
  //Restack old mags
  GUN_Magazine pmags1[GUN_MagazineCount]; //New mag stack
  for(int i=1;i<GUN_MagazineCount;i++) //Index 0 stands for the loaded magazine.
  {
    pmags1[i-1]=GUN_Magazines[i]; //Stack up the remaining magazines
  }
  pmags1[GUN_MagazineCount-1]=GUN_Magazines[0]; //Move the unloaded magazine to the end of the stack

  for(int i=0;i<GUN_MagazineCount;i++)
  {
    GUN_Magazines[i]=pmags1[i];
  }

  //Play reloading sound
  
  
  


  delay(5*1000); //Wait 5 seconds
  //Todo: Reassign the time for reloading
  
}

//Check incoming signals
void EXT_SensorCheck()
{

  //Check sensor state by reading the signals
  int SensorState=digitalRead(GUN_Sensor)||digitalRead(EXT_Sensor1)||digitalRead(EXT_Sensor2)||digitalRead(EXT_Sensor3)||digitalRead(EXT_Sensor4);
  if(SensorState=HIGH) //If the sensor has been lit
    SYS_Hit(); //Ouch!!!
  //TODO: Decode the MilesTag 2 signal

  //[Header]-[0ppppppp]-[ttdddd]
  //Header: 2400 us HIGH, 600 us LOW
  //Zero: 600 us HIGH, 600 us LOW
  //One: 1200 us HIGH, 600 us LOW

}

//Receive hit
void SYS_Hit()
{
  //Fire the LED and the vibro for half a second
  digitalWrite(EXT_Vibro, HIGH);
  delay(500);
  digitalWrite(EXT_Vibro, LOW);

  //Say "I'm hit!!!"
  //Subtract the HP damage
  //Stun the player
  
}

//Die
void SYS_Dead()
{

  //Fire the LED and the vibro for ten seconds.
  digitalWrite(EXT_Vibro, HIGH);
  delay(10000);
  digitalWrite(EXT_Vibro, LOW);

  //Disable the player until respawn
  
}





////////////////
// MAIN CODE! //
////////////////
void loop() {
  // put your main code here, to run repeatedly:


  //Check sensors for incoming signal
  EXT_SensorCheck();
  //Operate the trigger
  GUN_Trigger();

  //TODO: Regenerate health after the specified time if not hit for certain time
  //TODO: Armour together with HP
  //TODO: Status effects i.e. bleeding, stun etc



}


/////////////////////////////////
// SLAVA UKRAINI HEROYAM SLAVA //
// PUTiN HUILO RUSSIA PARUSSiA //
/////////////////////////////////
