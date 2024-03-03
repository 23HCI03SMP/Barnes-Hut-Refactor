#include "include/barnes.h"

Node::Node(Charge charge)
{
    this->charge = charge;
}

bool Node::isExternalNode()
{
    bool isParticle = dynamic_cast<Particle *>(this);
    
    bool isExternal;
    if (!isParticle)
    {
        if (dynamic_cast<Space *>(this)->children.size() == 0)
        {
            isExternal = true;
        }
        else
        {
            isExternal = false;
        }
    }

    return isParticle || isExternal;
}