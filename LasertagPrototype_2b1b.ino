#include "Wait.h"
#include "BubbleSort.h"
#include <IRremote.hpp>

//////////
// Laser tag gun prototype

//TODO: Pinout
/*
Pinout:
0-RXD
1-TXD
2-Muzzle flash
3-IR emitter
4-Trigger
5-Reload
6-Selective fire
7-Vibration/LED (to sensors)
8-Sensor signal
9-
10-
11-
12-
13-
*/
int GUN_FlashPin=2; //Muzzle flash pin
IRsend emitter(3); //Emitter for the gun
int GUN_TriggerPin=4; //Trigger
int GUN_Reload=5; //Reload pin
int GUN_FireModePin=6;//Selective fire mode, 0 - semi, 1 - auto
int GUN_VibroLEDPin=7;

IRrecv sensor(8); //Gun sensor signal
decode_results results;




//Player, team
int playerid=95; //Player ID 0-127
int team=2; //Team ID 0-3
//Gun damage
int damage=10; //Gun damage 0-15
//TODO: Implement player ID, team ID and damage configuration

int GUN_FireMode;//0 - semi, 1 - auto

int hp=100;//Health
//Todo: Implement damage
bool hit=false;

//Is the game active?
bool game_active=false;
//Is the player alive?

//Todo: Implement magazines
//int ammo=30;

int mag_count=3; //Total amount of magazines
int mag_capacity=30; //Capacity of each magazine
int ammo[10];//Max magazines - 10 (currently for testing)
//Note: C++ array index must be constant, mag_count shouldn't be less than array size!!!

//Total amount of ammo in all magazines
int ammo_total()
{
  int a;
  for(int i=0;i<mag_count;i++)
  {
    a+=ammo[i];
  }
  return a;
}

//Rearm magazines by providing full ammo to player
void FullAmmo()
{
  for(int i=0;i<mag_count;i++)
  {
    ammo[i]=mag_capacity;
  }
}

//Starts new game
void StartNewGame()
{
  //Set the game to active
  game_active=true;
  //Give full ammo to the player
  FullAmmo();
  //Full HP
  hp=100;
}

int firerate=600; //Fire rate, rounds per minute


void setup() {
  // put your setup code here, to run once:
  //Test
  pinMode(GUN_FlashPin,OUTPUT);//Muzzle flash
  pinMode(GUN_TriggerPin,INPUT);//Trigger
  pinMode(GUN_Reload,INPUT);//Reload
  pinMode(GUN_FireModePin,INPUT);//Select fire
  pinMode(GUN_VibroLEDPin,OUTPUT);
  sensor.enableIRIn();


  //Debug
  pinMode(13,OUTPUT);

  //Fill magazines up initially
  FullAmmo();

  //pinMode(7,OUTPUT);
  Serial.begin(9600);

}

//Fire a gun
void GUN_Fire()
{
      //this works!
      //Gunfire code
      //0ppppppp-ttdddd
      //Todo:Implement MilesTag 2 protocol as a Sony variation
      digitalWriteFast(GUN_FlashPin,HIGH);
      unsigned long packet = (((playerid << 2) | team) << 4) | damage;
      emitter.sendSony(packet, 14);//Send as Sony SIRC (MilesTag 2 is Sony SIRC variant with MSB first)
      digitalWriteFast(GUN_FlashPin,LOW);
      Wait((int)((1.0/((float)firerate/60.0))*1000.0));//Gun fire rate
      ammo[0]-=1;//Round away

      int i=ammo[0];
      Serial.print(i);Serial.print("/");Serial.println(ammo_total());
}

void GUN_Trigger()
{
  bool hasfired=false;
  if(GUN_FireMode==1)//If full auto
  {
    if(digitalRead(GUN_TriggerPin)==HIGH)
    {
      if(ammo[0]>0)
      {
        GUN_Fire();
      }
    }

  }
  else //If semi
  {
    while(digitalRead(GUN_TriggerPin)==HIGH)
    {
      if(ammo[0]>0)
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

//Hit points value (0-100) to binary damage value (4 bits)
int SYS_HP2Damage(int damage)
{
  switch(damage)
  {
    case 0 ... 1:
      return 0;
    case 2 ... 3:
      return 1;
    case 4:
      return 2; 
    case 5 ... 6:
      return 3;
    case 7 ... 9:
      return 4;
    case 10 ... 14:
      return 5;
    case 15 ... 16:
      return 6;
    case 17 ... 19:
      return 7;
    case 20 ... 24:
      return 8;
    case 25 ... 29:
      return 9;
    case 30 ... 34:
      return 10;
    case 35 ... 39:
      return 11;
    case 40 ... 49:
      return 12;
    case 50 ... 74:
      return 13;
    case 75 ... 99:
      return 14;
    case 100:
      return 15;
  }
}
//Binary damage value (4 bits) to hit points (0-100)
//Takes lowest amount
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

//If the player is dead
void SYS_Dead()
{
  hp=0; //Dead!
  //10 second vibration
  digitalWrite(GUN_VibroLEDPin,HIGH);
  Wait(10000);
  digitalWrite(GUN_VibroLEDPin,LOW);

}

void SYS_Respawn()
{
  hp=100;
}

void loop() {
  // put your main code here, to run repeatedly:


  //digitalWrite(7,GUN_FireMode);

  //Debug
  digitalWrite(13,game_active);


  //Run the game.
  //If the game is active
  if(game_active)
  {
    //Check firemode (HIGH if full auto)
    GUN_FireMode=digitalRead(GUN_FireModePin);

    //Gun fires and reloads only if the player is alive.
    //If player is alive
    if(hp>0)
    {
      //Trigger operation
      GUN_Trigger();
      //Reloading the gun
      if(digitalRead(GUN_Reload)==HIGH)
      {
        //Serial.print(ammo);
        //Resort the magazines by using bubble sort
        BubbleSort(ammo,mag_count);

      }
    }
    //Hit indication
    if(hit)
    {
      //If player is alive
      if(hp>0)
      {
        //1 second vibration/indication
        digitalWrite(GUN_VibroLEDPin,HIGH);
        Wait(1000);
        digitalWrite(GUN_VibroLEDPin,LOW);
      }
      //If dead
      else
      {
        hp=0; //Dead!
        //10 second vibration
        digitalWrite(GUN_VibroLEDPin,HIGH);
        Wait(10000);
        digitalWrite(GUN_VibroLEDPin,LOW);        
      }
      //Return hit
      hit=!hit;
    }


  }


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
      Serial.println(packet,HEX);
      if((packet&E8_mask)==0xE8)
      {
        //Incoming message ID. Should be equal or greater than 0x80 since the first (most significant) bit is 1.
        unsigned long incoming_message=(packet&0b111111110000000000000000)>>16;
        //Incoming message value.
        unsigned long incoming_value=(packet&0b1111111100000000)>>8;


        //Decode a service message
        Serial.println(results.value, BIN);
        Serial.print("Message: ");Serial.println(incoming_message,HEX);
        Serial.print("Value: ");Serial.println(incoming_value,HEX);

        //Process messages

        //0x80 - Add health 0-100
        //0x80xxE8, where xx=0x01 to 0x64 (0-100)
        if(incoming_message==0x80)
        {

          hp+=incoming_value;

          Serial.print("Add health ");Serial.println(incoming_value);

        }
        //0x81 - Add ammo
        //0x81xxE8, where xx=0x01 to 0x64 (0-100)
        if(incoming_message==0x81)
        {
          int ammo2=incoming_value;
          //Take each magazine
          for(int i=0;i<mag_count;i++)
          {
            //If magazine is full already
            if(ammo[i]==mag_capacity)
              i++;//Move on
            else //If empty
            {
              //How many rounds should we add? Obviously less than already is there! 
              int ammo3=mag_capacity-ammo[i];
              ammo2-=ammo3; //Take away rounds to add
              //If out of ammo to add
              if(ammo2<=0)
                break;
              else
                ammo[i]+=ammo3; //Add the lacking rounds




            }

          }


          Serial.print("Add ammo ");Serial.println(incoming_value);
        }
        //0x82 - Reserved
        //0x83 - Command
        //Commands
        if(incoming_message==0x83)
        {
          //Admin kill
          //0x8300E8
          if(incoming_value==0x00)
          {
            //temp code
            hp=0; //Kill the player
            Serial.println("Admin kill");
          }
          //Pause/Unpause
          //0x8301E8
          if(incoming_value==0x01)
          {

            Serial.println("Pause/Unpause");
          }
          //Start game (delayed)
          //0x8302E8
          if(incoming_value==0x02)
          {
            //TODO: What delay???
            StartNewGame();
            //////////////
            Serial.println("Start game (delayed)");
          }
          //Restore default settings
          //0x8303E8
          if(incoming_value==0x03)
          {

            Serial.println("Factory reset (restore default settings)");
          }
          //Respawn
          //0x8304E8
          if(incoming_value==0x04)
          {
            //Start with full hit points without shutting the game down
            hp=100;
            Serial.println("Respawn");
          }
          //Start game (immediately)
          //0x8305E8
          if(incoming_value==0x05)
          {
            //Immediately
            StartNewGame();
            ///////////////////
            Serial.println("Start game (immediately)");
          }
          //Full ammo
          //0x8306E8
          if(incoming_value==0x06)
          {
            //Rearm the player's magazines
            FullAmmo();
            Serial.println("Full ammo");
          }
          //End game (game over)
          //0x8307E8
          if(incoming_value==0x07)
          {
            //Temp code
            game_active=false;
            Serial.println("Game over");
          }
          //Reset clock
          //0x8308E8
          if(incoming_value==0x08)
          {

            Serial.println("Reset clock");
          }
          //0x8309E8 - Reserved

          //Initialize player
          //0x830AE8
          if(incoming_value==0x0A)
          {

            Serial.println("Initialize player");
          }
          //Explosion
          //0x830BE8
          if(incoming_value==0x0B)
          {
            //temp code
            hp=0;//Kill the player
            //Todo: Explosion sound

            Serial.println("Explosion");
          }
          //Initialize new game
          //0x830CE8
          if(incoming_value==0x0C)
          {

            Serial.println("Initialize new game");
          }
          //Full HP
          //0x830DE8
          if(incoming_value==0x0D)
          {
            //temp code
            hp=100;

            Serial.println("Full health");
          }
          //0x830EE8 - Reserved
          //Full armour
          //0x830FE8
          if(incoming_value==0x0F)
          {
            //TODO: Armour
            /////////////
            Serial.println("Full armour");
          }
          //0x8310E8 - Reserved
          //0x8311E8 - Reserved
          //0x8312E8 - Reserved
          //0x8313E8 - Reserved
          //Reset score
          //0x8314E8
          if(incoming_value==0x14)
          {

            Serial.println("Reset score");
          }
          //Sensor test
          //0x8315E8
          if(incoming_value==0x15)
          {

            //Blink and vibrate for 1 second

            digitalWrite(GUN_VibroLEDPin,HIGH);
            Wait(1000);
            digitalWrite(GUN_VibroLEDPin,LOW);

            Serial.println("Sensor test");



          }
          //Stun player
          //0x8316E8
          if(incoming_value==0x16)
          {

            Serial.println("Stun player");
          }
          //Disarm player (set ammo to 0)
          if(incoming_value==0x17)
          {

            for(int i=0;i<mag_count;i++)
            {
              ammo[i]=0;
            }

            Serial.println("Disarm player");
          }
        }

      }
      else
      {
        //Invalid packet!
        Serial.print("Invalid message packet!!!");
      }
      sensor.resume();
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

      //If game is active
      if(game_active)
      {
        //Indicate hit
        hit=true;
        //Subtract hit points
        hp-=SYS_Damage2HP(incoming_damage);
        //TODO: Armour implementation
        if(hp<0) hp=0;
        Serial.print("Remaining HP: ");Serial.println(hp);
      }
    
      sensor.resume();
    }
    else
    {
      //Invalid packet
      Serial.println(packet,BIN);
      Serial.println("Invalid packet!!!");      
      sensor.resume();
    }
  }
}
