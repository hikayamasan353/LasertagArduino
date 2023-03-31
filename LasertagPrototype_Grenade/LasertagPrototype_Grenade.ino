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

int GREN_Emitter; //Common pin for all emitters. Connected to the ground.
int GREN_Safety;  // Safety pin


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
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(GREN_Safety=LOW)
  {
    delay(5000); //5 seconds
    GREN_Detonate();
  }
  

}
