#include "include/barnes.h"

BaseParticle::BaseParticle(std::string alias, double mass, Charge charge) : Node(charge)
{
    this->alias = alias;
    this->mass = mass;
}