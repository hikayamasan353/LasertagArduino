/////////////////////////////
// KOZAC Grenade/Flashbang //
/////////////////////////////
#include "IRremote.hpp"


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

//Common pin for all emitters. Connected to the ground.
IRsend GREN_Emitter(3);
int GREN_Safety=4;  // Safety pin
bool detonated=false;//Has the grenade been detonated?


void setup() {
  // put your setup code here, to run once:
  pinMode(GREN_Safety, INPUT);

}

//Fire the grenade
void GREN_Detonate()
{
  //MilesTag 2 explode player
  // [Header]-0x83-0x0B-0xE8
  unsigned long packet=((0x83<<8)&0x0b)<<8&0xE8;
  GREN_Emitter.sendSony(packet,24); //sony SIRC have same timings as MT2
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
