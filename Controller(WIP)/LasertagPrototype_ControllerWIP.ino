#include <IRremote.hpp>


//Laser tag controller WIP
//Currently just two buttons: new game and game over

int IRpin=3;
IRsend irsend(IRpin);

//TODO: Buttons
int NewGamePin=2;
int GameOverPin=4 ;



void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(NewGamePin,INPUT);
  pinMode(GameOverPin,INPUT);
  //pinMode(3,OUTPUT);
  //pinMode(13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(NewGamePin)==HIGH)
  {
    //New game (immediately)
    //0x8305E8
    irsend.sendSony(0x8305E8,24);
    delay(100);
  }
  if(digitalRead(GameOverPin)==HIGH)
  {
    //New game (immediately)
    //0x8307E8
    irsend.sendSony(0x8307E8,24);
    delay(100);
  }

  //if((digitalRead(NewGamePin)==HIGH)&&(digitalRead(GameOverPin)==HIGH))
  //if(digitalRead(NewGamePin)==HIGH)
  //digitalWrite(3,HIGH);
  //else digitalWrite(3,LOW);

}
