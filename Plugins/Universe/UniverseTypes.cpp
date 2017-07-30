#include "UniverseTypes.h"

Universe::Universe(const double x, const double y, Bin* const bin)
 : Entity::Entity(x, y, bin), //Needs no real location on the map
   orbitDiameter(orbitRadius*6.28), orbitSpeed(1.0/sqrt(double(orbitRadius^3)/( exp(6.67408*10, -11)*double((planetMass)+(sunMass)))))
{

}

void Universe::update(const unsigned int resolution)
{
  //Rotate, and to keep within sane bounds, modulate by tau
  currentRoation = (currentRoation + (rotationSpeed*resolution)) % 6.28;
  currentOrbit   = (currentOrbit + (orbitSpeed*resolution)) % 6.28;


}
