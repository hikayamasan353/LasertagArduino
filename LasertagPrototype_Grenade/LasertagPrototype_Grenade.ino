/////////////////////////////
// KOZAC Grenade/Flashbang //
/////////////////////////////
#include "MilesTag2.h"


////////////////////////////////////////////////////
// Grenade has multiple LED emitters and a sound  //
// speaker for the explosion imitation.           //
// In flashbang mode, the emitter pin is used for //
// LED flash that is strong enough to cause       //
// floaters. USE WITH CAUTION AND SUPERVISION!!!  //
////////////////////////////////////////////////////
// POWER  //
// Use 9V //
////////////

int GREN_Emitter=3; //Common pin for all emitters. Connected to the ground.
//Should be PWM!!!
int GREN_Safety=4;  // Safety pin
bool detonated=false;//Has the grenade been detonated?


void setup() {
  // put your setup code here, to run once:
  pinMode(GREN_Emitter, OUTPUT); //
  pinMode(GREN_Safety, INPUT);

}

//Fire the grenade
void GREN_Detonate()
{
  //MilesTag 2 explode player
  // [Header]-0x83-0x0b
  MT2_Message(GREN_Emitter, SYS_MT2_Command, SYS_MT2_ExplodePlayer);
  //MT2_Message(GREN_Emitter, 0x83, 0x0b);
  // TODO: 0x83-0x0b
  //TODO: BANG!!!! BOOOM!!! BAMMMM!!!!
  
}

//Reset the grenade by putting safety back in
void GREN_Reset()
{
  detonated = false;
  while (digitalRead(GREN_Safety) == HIGH) {
    // Wait for safety pin to be pulled
    delay(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(GREN_Safety==LOW&&!detonated)
  {
    delay(5000); //5 seconds
    GREN_Detonate();
    detonated=true;
  }
  //To reprime the grenade by putting the safety pin back
  else if(detonated&&GREN_Safety==HIGH)
      GREN_Reset();
  

}
