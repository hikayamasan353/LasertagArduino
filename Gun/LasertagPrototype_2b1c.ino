#include "Wait.h"
#include "BubbleSort.h"
#include <IRremote.hpp>



//For debug
#define DEBUG

//For configurable settings via serial
#define DEBUGCLI

unsigned long SYS_last_time2=0;

//////////
// Laser tag gun prototype

//////////////////////////////////////////////////
// POWER                                        //
// Use airsoft grade Li-Ion or LiPo battery.    //
// Connect positive to VIN, negative to ground. //
//////////////////////////////////////////////////
/*
Pinout:
0 - RXD
1 - TXD
2 - Muzzle flash
3 - IR emitter
4 - Trigger
5 - Reload
6 - Selective fire
7 - Vibration/LED (to sensors)
8 - Sensor signal
9-
10-
11-
12-
13-
*/
//Connect to the emitter
#define GUN_FlashPin 2 //Muzzle flash pin
//IRsend emitter(3); //Emitter for the gun
#define GUN_EmitterPin 3 //Emitter for the gun
//Gun function
#define GUN_TriggerPin 4 //Trigger
#define GUN_ReloadPin 5 //Reload pin
#define GUN_FireModePin 6 //Selective fire mode, 0 - semi, 1 - auto

//Connect to the sensor
#define GUN_VibroLEDPin 7 //Hit indication signal

//IrReciever sensor(8); //Gun sensor signal
#define GUN_ReceiverPin 8
decode_results results;



////////////////////
// Gun parameters //
////////////////////

//Player ID 0-127
int playerid=95;

//Team ID 0-3
int team=2; 

//Gun damage 0-15
int damage=10; 
//TODO: Implement player ID, team ID and damage configuration

//Gun fire mode.
//0 - semi, 1 - auto
int GUN_FireMode=0; //Initialized as semi

//Health.
//If 0 hp - you're dead.
int hp=100;

//Is the player hit?
//If yes, it fires the hit indication.
bool hit=false; 

// Is the game active?
// Gun will fire only if the game is active,
// and the player has more than zero HP.
bool game_active=false;


//Is the player alive?

//int ammo=30;



////////////////////////
// Magazine system    //
////////////////////////

//Total amount of magazines
int mag_count=3;
//Capacity of each magazine
int mag_capacity=30; 

//Bulk ammo array.
//Index 0 is for the locked and loaded magazine.
//Max magazines - 10 (currently for testing)
int ammo[10];
//Note: C++ array index must be constant, mag_count shouldn't be less than array size!!!

//Total amount of ammo in all magazines.
int ammo_total()
{
  int a;
  //Take each magazine
  for(int i=0;i<mag_count;i++)
  {
    //Add the magazine's amount of rounds
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


void setup()
{
  // put your setup code here, to run once:
  
  /////////////////////////////////////
  //pinModes
  pinMode(GUN_FlashPin,OUTPUT);//Muzzle flash
  pinMode(GUN_TriggerPin,INPUT);//Trigger
  pinMode(GUN_ReloadPin,INPUT);//Reload
  pinMode(GUN_FireModePin,INPUT);//Select fire
  pinMode(GUN_VibroLEDPin,OUTPUT);
  //sensor.enableIRIn();
  IrSender.begin(GUN_EmitterPin);
  IrReceiver.begin(GUN_ReceiverPin);
  ///////////////////////////////////


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
      //Todo:Implement MilesTag 2 protocol as a Sony variation in an IRremote extension
      digitalWriteFast(GUN_FlashPin,HIGH);//Fire a muzzle flash on

      //Construct IR data packet:
      //7 bits player ID 0-127 (first bit always 0!!!),
      //2 bits team ID 0-3,
      //4 bits damage 0-15.
      unsigned long packet = (((playerid << 2) | team) << 4) | damage;

      

      //Send as Sony SIRC (MilesTag 2 is Sony SIRC variant with MSB first)
      IrSender.sendSonyMSB(packet, 14);

      //Debug (bit amount)
      //Serial.print(checkBitSize(packet));Serial.print(" bits");Serial.println();

      //Muzzle flash off
      digitalWriteFast(GUN_FlashPin,LOW);
      //Gun fire rate delay
      Wait((int)((1.0/((float)firerate/60.0))*1000.0));//Gun fire rate delay
      //Round away
      ammo[0]-=1;

      int i=ammo[0];

      //Debug
      #ifdef DEBUG
      Serial.print(i);Serial.print("/");Serial.println(ammo_total());
      #endif
}

//Has the gun fired? (For semi auto)
bool hasfired=false; //SOLVED: Cheating shouldn't be the issue

//Gun trigger operation
void GUN_Trigger()
{
  
  //If full auto
  if(GUN_FireMode==1)
  {
    //If the trigger is pressed
    if(digitalRead(GUN_TriggerPin)==HIGH)
    {
      //Fire the gun continuously while there is ammo
      if(ammo[0]>0)
      {
        GUN_Fire();
      }
    }

  }

  //If semi
  else
  {
    //If the trigger is pressed
    if(digitalRead(GUN_TriggerPin)==HIGH)
    {
      //If there's ammo
      if(ammo[0]>0)
      {
        //If the gun hasn't fied yet
        if(!hasfired)
        {
          //Fire!
          GUN_Fire();
          hasfired=true; //The gun has already fired
        }
        //If the gun is already fired
        else
        {
          //Do not fire
          hasfired=true;//It has already fired!
        }
      }

    }
    //On trigger release
    else
    {
      hasfired=false; //Gun can fire again
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

//Kill the player and fire death indication
void SYS_Dead()
{
  hp=0; //Dead!
  //10 second vibration
  digitalWrite(GUN_VibroLEDPin,HIGH);
  Wait(10000);
  digitalWrite(GUN_VibroLEDPin,LOW);

}

//Respawn the player without restarting game
void SYS_Respawn()
{
  //TODO: Respawning sfx
  hp=100;
}

void loop() 
{




  //Debug: Game activation
  #ifdef DEBUG
  digitalWrite(13,game_active);
  #endif

  //CLI debug test code
  #ifdef DEBUGCLI
  if(Serial.available())
  {
    String s=Serial.readString();
    s.trim();
    Serial.println(s);

    if(s=="gundata")
    {
      Serial.print("Player ID: ");
      Serial.println(playerid);

      Serial.print("Team: ");
      Serial.println(team);

      Serial.print("Damage: ");
      Serial.println(damage);
    }

    if(s=="activate")
      game_active=true;
    else if(s=="gameover")
      game_active=false;

    

  }

  #endif


  //Run the game.
  //If the game is active
  if(game_active)
  {
    //Check firemode (HIGH if full auto)
    GUN_FireMode=digitalRead(GUN_FireModePin);

    //Gun fires and reloads only if the player is alive!
    //If player is alive
    if(hp>0)
    {
      //Is the gun able to shoot?
      bool canshoot;
      //Is the gun being reloaded?
      bool reloading=false;
      //Is the mag empty? OR Is the gun being reloaded?
      if((ammo[0]==0)||(reloading))
        canshoot=false;
      else
        canshoot=true;
      
      //Can you fire?
      if(canshoot&&!reloading)
      {
        //Trigger operation
        GUN_Trigger();
      }


      //Reloading the gun
      if(digitalRead(GUN_ReloadPin)==HIGH)
      {
        reloading=true;
        //Is the reload successful?
        bool reloadsuccess;
        //Modified wait() code
        //Wait for 5 seconds
        unsigned long start_time = millis();

        //Debug code
        #ifdef DEBUG
        Serial.println("Reloading...");
        #endif

        //TODO: Reload start SFX
        while (millis() - start_time < 5000)
        {
            //If sensor input (getting hit)
            //if(sensor.decode_old(&results))
            if(IrReceiver.decode())
            {
              //Reload unsuccessful!
              reloadsuccess=false;
              //Break out of the loop
              break;
              //Do not reload
            }
            else
            {
              //Successful reload
              reloadsuccess=true;
            }
        }
        SYS_last_time = millis();
        //If reload is successful
        if(reloadsuccess)
        {
          //Todo: Reload success SFX
          BubbleSort(ammo,mag_count);
          reloading=false;

          //Debug macro
          #ifdef DEBUG
          Serial.println("Reloaded!");
          #endif
        }


        
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
        //Player is dead.
        SYS_Dead();   
      }
      //Return hit
      hit=!hit;
    }


  }


  //Check sensors
  //this works!!!
  if(IrReceiver.decode())
  {
    //Received IR data packet
    //unsigned long packet=results.value;

    //Recent updates to IRremote library have made LSB first decoding by default. 
    unsigned long packet=IrReceiver.decodedIRData.decodedRawData;

    ////////////////////////////////
    // Service message is 24 bits //
    // 1mmmmmmm-nnnnnnnn-0xE8     //
    ////////////////////////////////

    //If the MSB is 1, and the packet is 24 bits (Message packet)
    if(((bitreverse32Bit(packet)>>(32-24))>=0x800000)&&((bitreverse32Bit(packet)>>(32-24))<=0xFFFFFF))
    {
      //Decode a service message
      //Check if it ends with 0xE8

      //3rd byte mask.
      unsigned long E8_mask=0b000000000000000011111111;
      //Debug

      #ifdef DEBUG
      Serial.println((bitreverse32Bit(packet)>>(32-24)),HEX);
      #endif

      //If the last byte is E8
      if(((bitreverse32Bit(packet)>>(32-24))&E8_mask)==0xE8)
      {

        //////////////////////////////
        // Decode a service message //
        //////////////////////////////

        //Incoming message ID.
        //Should be equal or greater than 0x80,
        //since the first (most significant) bit in message packets is always 1.
        unsigned long incoming_message=((bitreverse32Bit(packet)>>(32-24))&0b111111110000000000000000)>>16;
        //Incoming message value.
        unsigned long incoming_value=((bitreverse32Bit(packet)>>(32-24))&0b1111111100000000)>>8;


        //Debug
        #ifdef DEBUG
        Serial.println(results.value, BIN);
        Serial.print("Message: ");Serial.println(incoming_message,HEX);
        Serial.print("Value: ");Serial.println(incoming_value,HEX);
        #endif

        //////////////////////
        // Process messages //
        //////////////////////

        //0x80 - Add health 0-100
        //0x80xxE8, where xx=0x01 to 0x64 (0-100)
        if(incoming_message==0x80)
        {

          //Add hit points
          hp+=incoming_value;

          //Max HP=100
          if(hp>=100)
            hp=100;
          //TODO: Larger HP values???

          //Debug
          #ifdef DEBUG
          Serial.print("Add health ");Serial.println(incoming_value);
          #endif

        }

        //0x81 - Add ammo
        //0x81xxE8, where xx=0x01 to 0x64 (0-100)
        if(incoming_message==0x81)
        {
          //Take the amount of ammo to be added
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

          //Debug
          #ifdef DEBUG
          Serial.print("Add ammo ");Serial.println(incoming_value);
          #endif


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
            //If the game is active
            if(game_active)
              SYS_Dead(); //Kill the player

            //Debug
            #ifdef DEBUG
            Serial.println("Admin kill");
            #endif
          }

          //Pause/Unpause
          //0x8301E8
          if(incoming_value==0x01)
          {
            //Debug
            #ifdef DEBUG
            Serial.println("Pause/Unpause");
            #endif
          }

          //Start game (delayed)
          //0x8302E8
          if(incoming_value==0x02)
          {
            //TODO: What delay???
            StartNewGame();
            //////////////

            //Debug
            #ifdef DEBUG
            Serial.println("Start game (delayed)");
            #endif
          }

          //Restore default settings
          //0x8303E8
          if(incoming_value==0x03)
          {
            //Debug
            #ifdef DEBUG
            Serial.println("Factory reset (restore default settings)");
            #endif
          }

          //Respawn
          //0x8304E8
          if(incoming_value==0x04)
          {
            //Start with full hit points without shutting the game down
            hp=100;

            //Debug
            #ifdef DEBUG
            Serial.println("Respawn");
            #endif
          }

          //Start game (immediately)
          //0x8305E8
          if(incoming_value==0x05)
          {
            //Immediately
            StartNewGame();
            ///////////////////

            //Debug
            #ifdef DEBUG
            Serial.println("Start game (immediately)");
            #endif
          }

          //Full ammo
          //0x8306E8
          if(incoming_value==0x06)
          {
            //Rearm the player's magazines
            FullAmmo();

            //Debug
            #ifdef DEBUG
            Serial.println("Full ammo");
            #endif
          }

          //End game (game over)
          //0x8307E8
          if(incoming_value==0x07)
          {
            //End the game session
            game_active=false;

            //Debug
            #ifdef DEBUG
            Serial.println("Game over");
            #endif
          }

          //Reset clock
          //0x8308E8
          if(incoming_value==0x08)
          {
            //How to even implement clock? What it even is???


            //Debug
            #ifdef DEBUG
            Serial.println("Reset clock");
            #endif
          }

          //0x8309E8 - Reserved

          //Initialize player
          //0x830AE8
          if(incoming_value==0x0A)
          {
            
            //Debug
            #ifdef DEBUG
            Serial.println("Initialize player");
            #endif
          }

          //Explosion kill
          //0x830BE8
          if(incoming_value==0x0B)
          {
            //If the game is active
            if(game_active)
              SYS_Dead();//Kill the player
              //Todo: Explosion sound

            //Debug
            #ifdef DEBUG
            Serial.println("Explosion");
            #endif
          }

          //Initialize new game
          //0x830CE8
          if(incoming_value==0x0C)
          {

            //Debug
            #ifdef DEBUG
            Serial.println("Initialize new game");
            #endif
          }

          //Full HP
          //0x830DE8
          if(incoming_value==0x0D)
          {
            //Restore the HP value
            hp=100;

            //Debug
            #ifdef DEBUG
            Serial.println("Full health");
            #endif
          }

          //0x830EE8 - Reserved

          //Full armour
          //0x830FE8
          if(incoming_value==0x0F)
          {
            //TODO: Armour
            /////////////

            //Debug
            #ifdef DEBUG
            Serial.println("Full armour");
            #endif
          }

          //0x8310E8 - Reserved

          //0x8311E8 - Reserved

          //0x8312E8 - Reserved

          //0x8313E8 - Reserved

          //Reset score
          //0x8314E8
          if(incoming_value==0x14)
          {

            //Debug
            #ifdef DEBUG
            Serial.println("Reset score");
            #endif
          }

          //Sensor test
          //0x8315E8
          if(incoming_value==0x15)
          {

            //Blink and vibrate for 1 second

            digitalWrite(GUN_VibroLEDPin,HIGH);
            Wait(1000);
            digitalWrite(GUN_VibroLEDPin,LOW);

            //Debug
            #ifdef DEBUG
            Serial.println("Sensor test");
            #endif



          }
          
          //Stun player
          //0x8316E8
          if(incoming_value==0x16)
          {

            //Debug
            #ifdef DEBUG
            Serial.println("Stun player");
            #endif
          }

          //Disarm player (set ammo to 0)
          //0x8317E8
          if(incoming_value==0x17)
          {

            for(int i=0;i<mag_count;i++)
            {
              ammo[i]=0;
            }

            //Debug
            #ifdef DEBUG
            Serial.println("Disarm player");
            #endif
          }

          //TODO: Add additional MilesTag 2-compatible protocol commands if any exist
        }

      }
      else
      {
        //Invalid packet!
        #ifdef DEBUG
        Serial.print("Invalid message packet!!!");
        #endif
      }
      IrReceiver.resume();
    }

    ///////////////////////////////
    // Gunfire packet is 14 bits //
    // 0ppppppp-ttdddd           //
    ///////////////////////////////

    //If the MSB is 0 and the packet size is 14 bits
    else if((bitreverse32Bit(packet)>>(32-14))<=0b01111111111111)
    {

      /////////////////////////////
      // Decode a gunfire packet //
      /////////////////////////////

      //Player ID 7 bits, 0-127
      int incoming_player=((bitreverse32Bit(packet)>>(32-14))&0b11111111000000)>>6;
      //Team ID 2 bits, 0-3
      int incoming_team=((bitreverse32Bit(packet)>>(32-14))&0b0000000110000)>>4;
      //Damage 4 bits, 0-15
      int incoming_damage=packet&0b0000000001111;

      //Debug
      #ifdef DEBUG
      Serial.println((bitreverse32Bit(packet)>>(32-14)), BIN);

      Serial.print("Player #");Serial.println(incoming_player);
      Serial.print("Team #");Serial.println(incoming_team);
      Serial.print(incoming_damage);Serial.println(" damage");
      #endif


      //Hit indication
      //Only if game is active
      if(game_active)
      {
        //Indicate hit (fire an LED and a vibration motor)
        hit=true;
        //Subtract hit points
        hp-=SYS_Damage2HP(incoming_damage);
        //TODO: Armour implementation
        if(hp<0) hp=0;

        //Debug code
        #ifdef DEBUG
        Serial.print("Remaining HP: ");Serial.println(hp);
        #endif
      }
    
      IrReceiver.resume();
    }
    else
    {
      //Invalid packet

      //Debug
      #ifdef DEBUG
      Serial.println(packet,BIN);
      Serial.println("Invalid packet!!!");
      #endif

      //Ignore the command     
      IrReceiver.resume();
    }
  }








}
