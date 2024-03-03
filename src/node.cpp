#include "include/barnes.h"

Node::Node(Charge charge)
{
    this->charge = charge;
}

bool Node::isExternalNode()
{  
    if (dynamic_cast<Particle *>(this)) return true;

    return dynamic_cast<Space *>(this)->children.size() == 0;
}