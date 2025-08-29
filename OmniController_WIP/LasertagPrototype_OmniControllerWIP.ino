//IR for laser tag
#include <IRremote.h>

//I2C
#include <Wire.h>

//1602 LCD
#include <LiquidCrystal_I2C.h> 

////////////////
// Components //
////////////////

//IR LED
#define IR_PIN 3
IRsend irsend(IR_PIN);

//Buttons
#define BTN_UP_PIN 4
#define BTN_DOWN_PIN 5
#define BTN_ENTER_PIN 2

//LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//////////////////////////////////
// MilesTag 2 protocol commands //
//////////////////////////////////

//Active command
int8_t command=0;
/*
Fully (WIP)
0 - Add health 100, 0x8064E8
1 - Add ammo 100, 0x8164E8
2 - Admin instakill, 0x8300E8
3 - Pause/Unpause, 0x8301E8
4 - Start game (delayed), 0x8302E8
5 - Factory reset, 0x8303E8
6 - Respawn, 0x8304E8
7 - Start game (immediately), 0x8305E8
8 - Full ammo, 0x8306E8
9 - Game over, 0x8307E8
10 - Reset clock, 0x8308E8
11 - Initialize player, 0x830AE8
12 - Explosion, 0x830BE8
13 - Ready game, 0x830CE8
14 - Full health, 0x830DE8
15 - Full armour, 0x830FE8
16 - Reset score, 0x8314E8
17 - Test sensors, 0x8315E8
18 - Stun player, 0x8316E8
19 - Disarm player, 0x8317E8
*/

//////////////////////////////
void setup()
{
  Serial.begin(9600);
  
  //Buttons
  pinMode(BTN_UP_PIN, INPUT); //Up
  pinMode(BTN_DOWN_PIN, INPUT); //Down
  pinMode(BTN_ENTER_PIN, INPUT); //Enter
  
  //LCD
  lcd.init(); //Initialize
  lcd.backlight(); //Turn backlight on
  

}

void loop()
{
  // put your main code here, to run repeatedly:

  //Select commands
  
  //Up
  if(digitalRead(BTN_UP_PIN)==HIGH)
  {
    lcd.clear();
    command+=1;
    if(command>19) command=0;
  }
  //Down
  if(digitalRead(BTN_DOWN_PIN)==HIGH)
  {
    lcd.clear();
    command-=1;
    if(command<0) command=19;
  }

  //Display command name
  switch(command)
  {
    //Add health 100
    case 0:
      {
        lcd.setCursor(0,0);
        lcd.print("8064E8");
        lcd.setCursor(0,1);
        lcd.print("ADD HEALTH 100");
        break;
      }
    
    //Add ammo 100
    case 1:
      {
        lcd.setCursor(0,0);
        lcd.print("8164E8");
        lcd.setCursor(0,1);
        lcd.print("ADD AMMO 100");
        break;
      }
    
    //Instakill
    case 2:
      {
        lcd.setCursor(0,0);
        lcd.print("8300E8");
        lcd.setCursor(0,1);
        lcd.print("ADMIN KILL");
        break;
      }
    
    //Pause/Unpause
    case 3:
      {
        lcd.setCursor(0,0);
        lcd.print("8301E8");
        lcd.setCursor(0,1);
        lcd.print("PAUSE");
        break;
      }
    
    //Start game delayed
    case 4:
      {
        lcd.setCursor(0,0);
        lcd.print("8302E8");
        lcd.setCursor(0,1);
        lcd.print("START GAME DLY");
        break;
      }
    
    //Factory reset
    case 5:
     {
        lcd.setCursor(0,0);
        lcd.print("8303E8");
        lcd.setCursor(0,1);
        lcd.print("FACTORY RESET");
        break;
      }

    //Respawn
    case 6:
      {
        lcd.setCursor(0,0);
        lcd.print("8304E8");
        lcd.setCursor(0,1);
        lcd.print("RESPAWN");
        break;
      }

    //Start game immediately
    case 7:
      {
        lcd.setCursor(0,0);
        lcd.print("8305E8");
        lcd.setCursor(0,1);
        lcd.print("START GAME");
        break;
      }

    //Full ammo
    case 8:
      {
        lcd.setCursor(0,0);
        lcd.print("8306E8");
        lcd.setCursor(0,1);
        lcd.print("FULL AMMO");
        break;
      }

    //Game over
    case 9:
      {
        lcd.setCursor(0,0);
        lcd.print("8307E8");
        lcd.setCursor(0,1);
        lcd.print("GAME OVER");
        break;
      }

    //Reset clock
    case 10:
      {
        lcd.setCursor(0,0);
        lcd.print("8308E8");
        lcd.setCursor(0,1);
        lcd.print("RESET CLOCK");
        break;
      }

    //Initialize player
    case 11:
      {
        lcd.setCursor(0,0);
        lcd.print("830AE8");
        lcd.setCursor(0,1);
        lcd.print("INIT PLAYER");
        break;
      }

    //Explosion
    case 12:
      {
        lcd.setCursor(0,0);
        lcd.print("830BE8");
        lcd.setCursor(0,1);
        lcd.print("EXPLOSION");
        break;
      }
    
    //Ready game
    case 13:
      {
        lcd.setCursor(0,0);
        lcd.print("830CE8");
        lcd.setCursor(0,1);
        lcd.print("READY GAME");
        break;
      }

    //Full health
    case 14:
      {
        lcd.setCursor(0,0);
        lcd.print("830DE8");
        lcd.setCursor(0,1);
        lcd.print("FULL HEALTH");
        break;
      }

    //Full armour
    case 15:
      {
        lcd.setCursor(0,0);
        lcd.print("830FE8");
        lcd.setCursor(0,1);
        lcd.print("FULL ARMOR");
        break;
      }

    //Reset score
    case 16:
      {
        lcd.setCursor(0,0);
        lcd.print("8314E8");
        lcd.setCursor(0,1);
        lcd.print("RESET SCORE");
        break;
      }

    //Test sensors
    case 17:
      {
        lcd.setCursor(0,0);
        lcd.print("8315E8");
        lcd.setCursor(0,1);
        lcd.print("TEST SENSORS");
        break;
      }

    //Stun
    case 18:
      {
        lcd.setCursor(0,0);
        lcd.print("8316E8");
        lcd.setCursor(0,1);
        lcd.print("STUN PLAYER");
        break;
      }

    //Disarm
    case 19:
      {
        lcd.setCursor(0,0);
        lcd.print("8317E8");
        lcd.setCursor(0,1);
        lcd.print("DISARM PLAYER");
        break;
      }
  }

  //Execute command
  if(digitalRead(BTN_ENTER_PIN)==HIGH)
  {
    switch(command)
    {
      //Add health 100
      case 0:
        {
          irsend.sendSony(0x8064E8,24);
          break;
        }
      
      //Add ammo 100
      case 1:
        {
          irsend.sendSony(0x8164E8,24);
          break;
        }
      
      //Instakill
      case 2:
        {
          irsend.sendSony(0x8300E8,24);
          break;
        }

      //Pause/Unpause
      case 3:
        {
          irsend.sendSony(0x8301E8,24);
          break;
        }
      
      //Start game (delayed)
      case 4:
        {
          irsend.sendSony(0x8302E8,24);
          break;
        }
      
      //Factory reset
      case 5:
        {
          irsend.sendSony(0x8303E8,24);
          break;
        }
      
      //Respawn
      case 6:
        {
          irsend.sendSony(0x8304E8,24);
          break;
        }

      //Start game (immediately)
      case 7:
        {
          irsend.sendSony(0x8305E8,24);
          break;
        }

      //Full ammo
      case 8:
        {
          irsend.sendSony(0x8306E8,24);
          break;
        }

      //Game over
      case 9:
        {
          irsend.sendSony(0x8307E8,24);
          break;
        }

      //Reset clock
      case 10:
        {
          irsend.sendSony(0x8308E8,24);
          break;
        }

      //Init player
      case 11:
        {
          irsend.sendSony(0x830AE8,24);
          break;
        }

      //Explosion
      case 12:
        {
          irsend.sendSony(0x830BE8,24);
          break;
        }
      
      //Ready game
      case 13:
        {
          irsend.sendSony(0x830CE8,24);
          break;
        }

      //Full health
      case 14:
        {
          irsend.sendSony(0x830DE8,24);
          break;
        }

      //Full armour
      case 15:
        {
          irsend.sendSony(0x830FE8,24);
          break;
        }

      //Reset score
      case 16:
        {
          irsend.sendSony(0x8314E8,24);
          break;
        }

      //Test sensors
      case 17:
        {
          irsend.sendSony(0x8315E8,24);
          break;
        }
      
      //Stun
      case 18:
        {
          irsend.sendSony(0x8316E8,24);
          break;
        }

      //Disarm
      case 19:
        {
          irsend.sendSony(0x8317E8,24);
          break;
        }
    }
  }
  
  //delay(10); // Delay a little bit to improve simulation performance
}