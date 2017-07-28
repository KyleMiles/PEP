#ifndef PLUGIN_UNIVERSE_DATA_FOR_PEP_ENVIRONMENT
#define PLUGIN_UNIVERSE_DATA_FOR_PEP_ENVIRONMENT 1

#include "../../Main/Entity.h"
#include <stdint.h>

//Cosmological data, for day/season/year/heatingRate calculations
class Universe : public Entity
{
public:
  Universe(const double x, const double y, Bin* const bin);

  void update(const unsigned int resolution) override;

private:
  //given constants
    //sun
    const int64_t sunMass = 1;       //kg
    //planet
    const int64_t planetRaduis = 1;  //cm
    const int64_t planetMass = 1;       //kg
    const int64_t orbitRadius = 1;   //cm
    const double axisTilt = 1;       //degrees
    const double rotationSpeed = 1;  //rad/sec

  //calculated constants
    const int64_t orbitDiameter;
    const double orbitSpeed;



  double currentRoation = 0;       //radians
  double currentOrbit = 0;         //radians
};

#endif
