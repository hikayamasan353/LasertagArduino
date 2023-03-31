#ifndef Magazine
#define Magazine
#include <Arduino.h>

// Virtual magazine for laser tag gun
class GUN_Magazine
{
  private:
  public:
    GUN_Magazine();
    GUN_Magazine(int p_capacity);
    GUN_Magazine(int p_capacity, int p_ammo);
    int ammo; //Total amount of ammo in magazine
    int capacity; //Magazine capacity

  
};

#endif

  GUN_Magazine::GUN_Magazine(){}
  //Creates a full magazine
  GUN_Magazine::GUN_Magazine(int p_capacity)
  {
    //Fill the magazine completely
    capacity=p_capacity;
    ammo=capacity;    
  }
  //Creates an unfilled magazine with defined ammo remaining
  GUN_Magazine::GUN_Magazine(int p_capacity, int p_ammo)
  {
    capacity=p_capacity;
    ammo=p_ammo;
  }
