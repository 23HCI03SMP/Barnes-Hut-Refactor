#include "barnes.h"

Space::Space(Point minPoint, Point maxPoint, double mass, Charge charge) : Node(mass, charge)
{
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;
    this->centreOfPositveCharge = Point(0, 0, 0);
    this->centreOfNegativeCharge = Point(0, 0, 0);
}

void Space::recalculateCentreOfCharge(Node *node)
{
    if (!dynamic_cast<Particle *>(node) || dynamic_cast<Space *>(node)->children.size() != 0)
    {
        double xPosPositiveChargeSum = 0;
        double yPosPositiveChargeSum = 0;
        double zPosPositiveChargeSum = 0;

        double xPosNegativeChargeSum = 0;
        double yPosNegativeChargeSum = 0;
        double zPosNegativeChargeSum = 0;

        double massSum = 0;
        double positiveChargeSum = 0;
        double negativeChargeSum = 0;

        for (Node *child : dynamic_cast<Space *>(node)->children)
        {
            if (!dynamic_cast<Particle *>(child))
            {
                recalculateCentreOfCharge(dynamic_cast<Space *>(child));

                
            }
            // if (!dynamic_cast<Particle *>(this->children[i]) || dynamic_cast<Space *>(this->children[i])->children.size() != 0)
        }
    }
}

bool Space::isExternalNode()
{
    return this->children.size() == 0;
}

bool Space::find(Point point)
{
    throw "Not implemented";
}

void Space::insert(Node node)
{
}