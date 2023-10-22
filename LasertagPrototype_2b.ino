#include "Wait.h"
#include <IRremote.hpp>

//TODO: Pinout
/*
Pinout:
0-
1-
2-
3-IR emitter
4-Trigger
5-Reload
6-Selective fire
7-
8-
9-
10-
11-
12-
13-
*/
IRsend emitter(3); //Emitter for the gun
int GUN_TriggerPin=4; //Trigger
int GUN_Reload=5; //Reload pin
int GUN_FireModePin=6;//Selective fire mode, 0 - semi, 1 - auto

IRrecv sensor(8); //Gun sensor signal
decode_results results;


//Player, team
int playerid=95; //Player ID 0-127
int team=2; //Team ID 0-3
//Gun damage
int damage=15; //Gun damage 0-15

int GUN_FireMode;//0 - semi, 1 - auto

int hp;//Health
//Todo: Implement damage

//Todo: Implement magazines
int ammo=30;

int firerate=600; //Fire rate, rounds per minute


void setup() {
  // put your setup code here, to run once:
  //Test
  pinMode(GUN_TriggerPin,INPUT);//Trigger
  pinMode(GUN_Reload,INPUT);//Reload
  pinMode(GUN_FireModePin,INPUT);//Select fire
  //Todo:Pinout
  sensor.enableIRIn();

  //pinMode(7,OUTPUT);
  Serial.begin(9600);

}

//Fire a gun
void GUN_Fire()
{
      //MT2_Fire(emitter,playerid,team,damage); does not work
      //this works!
      //Gunfire code
      //0ppppppp-ttdddd
      //Todo:Implement MilesTag 2 protocol as a Sony variation
      unsigned long packet = (((playerid << 2) | team) << 4) | damage;
      emitter.sendSony(packet, 14);//Send as Sony SIRC
      Wait((int)((1.0/((float)firerate/60.0))*1000.0));//Gun fire rate
      ammo-=1;//Round away
}

void GUN_Trigger()
{
  bool hasfired=false;
  while(digitalRead(GUN_TriggerPin)==HIGH)
  {
    if(ammo>0)
    {
      if(GUN_FireMode==1)
      {
          GUN_Fire();
      }
      else
      {

        if(!hasfired)
        {
          GUN_Fire();
          hasfired=true;
        }
        else
        {
          hasfired=true;
        }
      }
    }
  }

}

int SYS_Damage2HP(int damage)
{
  switch(damage)
  {
    case 0:
      return 1;
    case 1:
      return 2;
    case 2:
      return 4;
    case 3:
      return 5;
    case 4:
      return 7;
    case 5:
      return 10;
    case 6:
      return 15;
    case 7:
      return 17;
    case 8:
      return 20;
    case 9:
      return 25;
    case 10:
      return 30;
    case 11:
      return 35;
    case 12:
      return 40;
    case 13:
      return 50;
    case 14:
      return 75;
    case 15:
      return 100;
  }
}

void SYS_Dead()
{

}

void SYS_Respawn()
{
  hp=100;
}

void loop() {
  // put your main code here, to run repeatedly:

  GUN_FireMode=digitalRead(GUN_FireModePin);
  //digitalWrite(7,GUN_FireMode);


//////////////////////////////////////////////////////
  //Trigger operation
  
  
  GUN_Trigger();
  

  if(digitalRead(GUN_Reload)==HIGH)//Reload;
  {
    ammo=30;
  }
  ////////////////////////////////////////////////////////

  //Check sensors
  //this works!!!
  if(sensor.decode_old(&results))
  {
    unsigned long packet=results.value;
    ////////////////////////////
    //Service message is 24 bits
    //1mmmmmmm-nnnnnnnn-0xE8
    ////////////////////////////

    if((packet>=0x800000)&&(packet<=0xFFFFFF))
    {
      //Decode a service message
      //Check if it ends with 0xE8
      unsigned long E8_mask=0b000000000000000011111111;
      if(packet&E8_mask==0xE8)
      {
        unsigned long incoming_message=(packet&0b111111110000000000000000)>>16;
        unsigned long incoming_value=(packet&0b1111111100000000)>>8;


        //Decode a service message
        Serial.println(results.value, BIN);
        Serial.print("Message: ");Serial.println(incoming_message,HEX);
        Serial.print("Value: ");Serial.println(incoming_value,HEX);

        //Todo: Process messages
      }
      else
      {
        //Invalid packet!
        Serial.println("Invalid packet!!!");
        sensor.resume();
      }

    }
    ////////////////////////////
    //Gunfire packet is 14 bits
    //0ppppppp-ttdddd
    ////////////////////////////
    else if((packet>=0b01000000000000)&&(packet<=0b01111111111111))
    {
      //Decode a gunfire packet
      int incoming_player=(packet&0b11111111000000)>>6;
      int incoming_team=(packet&0b0000000110000)>>4;
      int incoming_damage=packet&0b0000000001111;

      Serial.println(results.value, BIN);

      Serial.print("Player #");Serial.println(incoming_player);
      Serial.print("Team #");Serial.println(incoming_team);
      Serial.print(incoming_damage);Serial.println(" damage");

      //Todo: Vibration motor
      //Todo: HP and respawn
    
      sensor.resume();
    }
    else
    {
      //Invalid packet
      Serial.println("Invalid packet!!!");
      sensor.resume();
    }
  }
}
