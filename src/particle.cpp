#include "barnes.h"
#include <string>

Particle::Particle(std::string alias, Point position, Velocity velocity, Force bForce, Force eForce, double mass, Charge charge) : Node(mass, charge)
{
    this->alias = alias;
    this->position = position;
    this->velocity = velocity;
    this->bForce = bForce;
    this->eForce = eForce;
}