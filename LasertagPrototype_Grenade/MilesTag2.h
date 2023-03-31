///////////////////////////
// KOZAC LASERTAG SYSTEM //
///////////////////////////
// MilesTag 2 library    //
// Ver. 0.1a             //
///////////////////////////
#include "Arduino.h"


/////////
// Prefixes
/////////
// MT2 - MilesTag 2 protocol
// SYS_MT2 - System MilesTag 2 protocol
////////

////////////////////////////////////////
//   IMPORTANT IMPORTANT IMPORTANT    //
////////////////////////////////////////
// First bit sent after the header is //
// differentiated between the gunshot //
// and the message type.              //
// It's a sign bit.                   //
////////////////////////////////////////

/////////////////
// Message IDs //
/////////////////
//Add health
const int SYS_MT2_AddHealth = 0x80;
//Add rounds
const int SYS_MT2_AddRounds = 0x81;


//Command. Use value for the command itself.
const int SYS_MT2_Command = 0x83;
////////////////////////////////
// COMMANDS // Use: 0x83 0xXX //
////////////////////////////////
// Admin Kill
const int SYS_MT2_AdminKill = 0x00;
// Pause/Unpause
const int SYS_MT2_Pause = 0x01;
// Start Game
const int SYS_MT2_StartGame = 0x02;
// Reset the gun to Factory Default
const int SYS_MT2_FactoryReset = 0x03;
// RESPAWN
const int SYS_MT2_RESPAWN = 0x04;
// New Game (IMMEDIATELY)
const int SYS_MT2_NewGame = 0x05;
// REARM
const int SYS_MT2_REARM = 0x06;
// Stop game. Prompt game over
const int SYS_MT2_GAMEOVER = 0x07;
// Reset clock
const int SYS_MT2_ResetClock = 0x08;
// Initialize player
const int SYS_MT2_InitPlayer = 0x0a;
// Explode player
const int SYS_MT2_ExplodePlayer = 0x0b;
// HEAL
const int SYS_MT2_HEAL = 0x0d;
// Full Armour
const int SYS_MT2_FullArmour = 0x0f;
// Reset score
const int SYS_MT2_ResetScore = 0x14;
// Test sensors
const int SYS_MT2_TestSensors = 0x15;
// STUN
const int SYS_MT2_STUN = 0x16;
// DISARM
const int SYS_MT2_DISARM = 0x17;

///////////////////////////////////
// System Data // Use: 0x87 0xXX //
///////////////////////////////////
const int SYS_MT2_SystemData=0x87;
////////////////////////////////////
// Cloning Data
const int SYS_MT2_CloningData=0x01;



//Clips pickup aka REARM2
// 0 to 15
const int SYS_MT2_REARM2=0x8a;
//Heal aka HEAL2
// 0 to 15
const int SYS_MT2_HEAL2=0x8b;




/////////////////////////////////
// LASERWAR Compatibility Code //
// Prefix: SYS_MT2_LW          //
/////////////////////////////////

//Change team ID
const int SYS_MT2_LW_TeamChange=0x09;
//Emitter power
const int SYS_MT2_LW_EmitterPower=0x0e;

//Point capture
const int SYS_MT2_LW_PointCapture=0x11;
//Transmitted by the point.
//////////////////////////////
//Defuse
const int SYS_MT2_LW_Defuse=0x12;




////////////
// Physical data


// Send a header
void MT2_Header(int pin)
{
  //2400 us high
  digitalWrite(pin, HIGH);
  delayMicroseconds(2400);
  //600 us low
  digitalWrite(pin, LOW);
  delayMicroseconds(600);  
}

// Binary Zero
void MT2_BIN_Zero(int pin)
{
  //600 us high
  digitalWrite(pin, HIGH);
  delayMicroseconds(600);
  //600 us low
  digitalWrite(pin, LOW);
  delayMicroseconds(600);
}
//Binary One
void MT2_BIN_One(int pin)
{
  //1200 us high
  digitalWrite(pin, HIGH);
  delayMicroseconds(1200);
  //600 us low
  digitalWrite(pin, LOW);
  delayMicroseconds(600);
}

//Sends hex values as binary pulses
void MT2_Signal(int pin, int bitlength, int value)
{
  //Fire each bit
  for(int i=0;i<bitlength;i++)
    {
      if(value&(1<<i)) //If bit reads one
        MT2_BIN_One(pin);
      else //If bit reads zero
        MT2_BIN_Zero(pin);
    } 
}


/*
//Player ID
//[0ppppppp]
void MT2_PlayerID(int pin, int value)
{
  int bitlength=7; //7 bit value, 0-127
  if(value>127)
  {
    Serial.println("ERROR: Invalid player ID! 0-127");
  }
  else
  {
    //Send zero as a sign bit
    MT2_BIN_Zero(pin);
    //Fire each bit
    for(int i=0;i<bitlength;i++)
    {
      if(value&(1<<i))
        MT2_BIN_One(pin);
      else
        MT2_BIN_Zero(pin);
    }
  }
}

//Team ID
//[tt]
*/



//////////////////////////////////
// MilesTag 2 Gun Fire          //
// [Header]-[0ppppppp]-[ttdddd] //
// p - Player ID BIN            //
// DEC: 0-127                   //
// t - Team ID BIN              //
// DEC: 0-3                     //
// d - Damage BIN               //
// DEC: 0-15                    //
//////////////////////////////////
void MT2_Fire(int pin, int PlayerID, int Team, int Damage)
{
  //Check the values
  if((PlayerID>127)||(Team>3)||(Damage>15))
  {
    //DEBUG: Call error
    Serial.println("Invalid value: PlayerID 0-127, Team 0-3, Damage 0-15");
  }
  else
  {
    //Fire a pulse
    //[Header]-[0ppppppp]-[ttdddd] 
    MT2_Header(pin); //Header
    //MT2_PlayerID(pin, PlayerID);
    MT2_BIN_Zero(pin); //Zero as a sign bit (it's a gunfire)
    MT2_Signal(pin, 7, PlayerID); //7 bit PlayerID
    MT2_Signal(pin, 2, Team); //2 bit Team ID
    MT2_Signal(pin, 4, Damage);    
  }
}

///////////////////////////////////////////
// MilesTag 2 Message                    //
// [Header]-[1mmmmmmm]-[dddddddd]-[0xE8] //
// m - Message ID BIN                    //
// d - Data values BIN                   //
// 0xE8 - OUT                            //
///////////////////////////////////////////
void MT2_Message(int pin, int MessageID, int Value)
{
  if((MessageID<128)||(Value>255))
  {
    //DEBUG: Call error
    Serial.println("Invalid value: Message ID 128-255, Value 0-255");
  }
  else
  {
    //Send a message
    // [Header]-[1mmmmmmm]-[dddddddd]-[0xE8]
    MT2_Header(pin); //Header
    //MT2_BIN_One; //One as message bit
    MT2_Signal(pin, 8, MessageID); //MessageID 8 bit
    MT2_Signal(pin, 8, Value); //Value 8 bit
    MT2_Signal(pin, 8, 0xE8); //Out   
  }
}




///////////////////////////////
// SLAVA UKRAINI PUTIN HUILO //
///////////////////////////////
