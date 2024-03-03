#include "include/barnes.h"
#include <iostream>
#include <vector>

#define o1 0 // top left back
#define o2 1 // top right back
#define o3 2 // top right front
#define o4 3 // top left front
#define o5 4 // bottom left back
#define o6 5 // bottom right back
#define o7 6 // bottom right front
#define o8 7 // bottom left front

Space::Space(Point minPoint, Point maxPoint, Charge charge) : Node(charge)
{
    this->minPoint = minPoint;
    this->maxPoint = maxPoint;
    this->centreOfPositveCharge = Point(0, 0, 0);
    this->centreOfNegativeCharge = Point(0, 0, 0);
}

Space::~Space()
{
    for (Node *child : this->getChildren())
    {
        delete child;
    }
}

std::vector<Particle *> Space::generateParticles(double density, Particle particle, double temperature, Shape shape, ...)
{
    throw "Not implemented";
}

std::vector<Particle *> Space::getChildren()
{
    std::vector<Particle *> particles;
    for (Node *child : this->children)
    {
        if (dynamic_cast<Particle *>(child))
        {
            particles.push_back(dynamic_cast<Particle *>(child));
        }
        else
        {
            std::vector<Particle *> childParticles = dynamic_cast<Space *>(child)->getChildren();
            particles.insert(particles.end(), childParticles.begin(), childParticles.end());
        }
    }
    return particles;
}

void Space::recalculateCentreOfCharge()
{
    if (!dynamic_cast<Particle *>(this) || dynamic_cast<Space *>(this)->children.size() != 0)
    {
        double xPosPositiveChargeSum = 0;
        double yPosPositiveChargeSum = 0;
        double zPosPositiveChargeSum = 0;

        double xPosNegativeChargeSum = 0;
        double yPosNegativeChargeSum = 0;
        double zPosNegativeChargeSum = 0;

        Charge chargeSum = Charge(0, 0);
        double positiveChargeSum = 0;
        double negativeChargeSum = 0;

        for (Node *child : dynamic_cast<Space *>(this)->children)
        {
            if (!dynamic_cast<Particle *>(child))
            {
                (dynamic_cast<Space *>(child))->recalculateCentreOfCharge();

                positiveChargeSum += child->charge.positive;
                negativeChargeSum += child->charge.negative;

                xPosPositiveChargeSum += dynamic_cast<Space *>(child)->centreOfPositveCharge.x * child->charge.positive;
                yPosPositiveChargeSum += dynamic_cast<Space *>(child)->centreOfPositveCharge.y * child->charge.positive;
                zPosPositiveChargeSum += dynamic_cast<Space *>(child)->centreOfPositveCharge.z * child->charge.positive;

                xPosNegativeChargeSum += dynamic_cast<Space *>(child)->centreOfNegativeCharge.x * child->charge.negative;
                yPosNegativeChargeSum += dynamic_cast<Space *>(child)->centreOfNegativeCharge.y * child->charge.negative;
                zPosNegativeChargeSum += dynamic_cast<Space *>(child)->centreOfNegativeCharge.z * child->charge.negative;
            }
            else
            {
                positiveChargeSum += child->charge.positive;
                negativeChargeSum += child->charge.negative;

                xPosPositiveChargeSum += dynamic_cast<Particle *>(child)->pos.x * child->charge.positive;
                yPosPositiveChargeSum += dynamic_cast<Particle *>(child)->pos.y * child->charge.positive;
                zPosPositiveChargeSum += dynamic_cast<Particle *>(child)->pos.z * child->charge.positive;

                xPosNegativeChargeSum += dynamic_cast<Particle *>(child)->pos.x * child->charge.negative;
                yPosNegativeChargeSum += dynamic_cast<Particle *>(child)->pos.y * child->charge.negative;
                zPosNegativeChargeSum += dynamic_cast<Particle *>(child)->pos.z * child->charge.negative;
            }

            chargeSum += child->charge;
        }

        this->charge = chargeSum;
        this->centreOfPositveCharge = positiveChargeSum == 0 ? Point() : Point(xPosPositiveChargeSum / positiveChargeSum, yPosPositiveChargeSum / positiveChargeSum, zPosPositiveChargeSum / positiveChargeSum);
        this->centreOfNegativeCharge = negativeChargeSum == 0 ? Point() : Point(xPosNegativeChargeSum / negativeChargeSum, yPosNegativeChargeSum / negativeChargeSum, zPosNegativeChargeSum / negativeChargeSum);
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

Space *CalculateSpaceParameters(Space *baseSpace, int octant)
{
    switch (octant)
    {
    case o1: // top left back
        return new Space(Point(baseSpace->minPoint.x, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z / 2), Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y, baseSpace->maxPoint.z), Charge(0, 0));
    case o2: // top right back
        return new Space(Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z / 2), baseSpace->maxPoint, Charge(0, 0));
    case o3: // top right front
        return new Space(Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y / 2, baseSpace->minPoint.z), Point(baseSpace->maxPoint.x, baseSpace->maxPoint.y, baseSpace->maxPoint.z / 2), Charge(0, 0));
    case o4: // top left front
        return new Space(Point(baseSpace->minPoint.x, baseSpace->maxPoint.y / 2, baseSpace->minPoint.z), Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z / 2), Charge(0, 0));
    case o5: // bottom left back
        return new Space(Point(baseSpace->minPoint.x, baseSpace->minPoint.y, baseSpace->maxPoint.z / 2), Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z), Charge(0, 0));
    case o6: // bottom right back
        return new Space(Point(baseSpace->maxPoint.x / 2, baseSpace->minPoint.y, baseSpace->maxPoint.z / 2), Point(baseSpace->maxPoint.x, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z), Charge(0, 0));
    case o7: // bottom right front
        return new Space(Point(baseSpace->maxPoint.x / 2, baseSpace->minPoint.y, baseSpace->minPoint.z), Point(baseSpace->maxPoint.x, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z / 2), Charge(0, 0));
    case o8: // bottom left front
        return new Space(baseSpace->minPoint, Point(baseSpace->maxPoint.x / 2, baseSpace->maxPoint.y / 2, baseSpace->maxPoint.z / 2), Charge(0, 0));
    default:
        throw "This shouldn't even happen (Octant shouldn't be anything other than o1 to o8)";
    }
}

int GetOctant(Particle *particle, Space *baseSpace)
{
    int octant;
    double xMid = (baseSpace->maxPoint.x + baseSpace->minPoint.x) / 2;
    double yMid = (baseSpace->maxPoint.y + baseSpace->minPoint.y) / 2;
    double zMid = (baseSpace->maxPoint.z + baseSpace->minPoint.z) / 2;

    if (particle->pos.x >= xMid) // Right
    {
        if (particle->pos.y >= yMid) // Top
        {
            if (particle->pos.z >= zMid) // Back
            {
                octant = o2; // top right back
            }
            else
            {
                octant = o3; // top right front
            }
        }
        else
        {
            if (particle->pos.z >= zMid) // Back
            {
                octant = o6; // bottom right back
            }
            else
            {
                octant = o7; // bottom right front
            }
        }
    }
    else // Left
    {
        if (particle->pos.y >= yMid) // Top
        {
            if (particle->pos.z >= zMid) // Back
            {
                octant = o1; // top left back
            }
            else
            {
                octant = o4; // top left front
            }
        }
        else // Bottom
        {
            if (particle->pos.z >= zMid) // Back
            {
                octant = o5; // bottom left back
            }
            else
            {
                octant = o8; // bottom left front
            }
        }
    }

    return octant;
}

void Space::insert(Node *node)
{
    // if the node to be added is a space
    if (dynamic_cast<Space *>(node))
    {
        for (Particle *child : dynamic_cast<Space *>(node)->getChildren())
        {
            this->insert(child);
        }

        delete node; // delete the space after all its children have been added
    }
    else if (dynamic_cast<Particle *>(node)) // if the node to be added is a particle
    {
        // if the space doesn't have any octants, create them
        if (this->children.size() == 0)
        {
            for (int i = o1; i <= o8; i++)
            {
                this->children.push_back(CalculateSpaceParameters(this, i));
            }
        }

        // decide which octant the node belongs to
        int octant = GetOctant(dynamic_cast<Particle *>(node), this);
        Node *octantNode = this->children[octant]; // get the octant node

        if (dynamic_cast<Space *>(octantNode)) // if the octant is a space
        {
            if (dynamic_cast<Space *>(octantNode)->isExternalNode()) // if the octant has no chidren, simply replace it with the node
            {
                delete octantNode;
                this->children[octant] = node;
            }
            else
            {
                dynamic_cast<Space *>(octantNode)->insert(node); // run recusively until an external node is found
            }
        }
        else // if the octant is a particle
        {
            Particle *insertParticle = dynamic_cast<Particle *>(node);
            Particle *currentPoint = dynamic_cast<Particle *>(octantNode);

            if ((insertParticle->pos.x == currentPoint->pos.x) && (insertParticle->pos.y == currentPoint->pos.y) && (insertParticle->pos.z == currentPoint->pos.z)) // if the coordinates of the particle to be added already exists
            {
                throw "Particle already exists. Boom! Fusion reaction!";
            }

            // Create a new space and insert the current particle and the new particle into the new space, along with other octants
            Space *newTree = CalculateSpaceParameters(this, octant);

            int insertOctant = GetOctant(insertParticle, newTree);
            int currentOctant = GetOctant(currentPoint, newTree);
            for (int i = o1; i <= o8; i++)
            {
                if (i == insertOctant)
                {
                    newTree->children.push_back(insertParticle);
                }
                else if (i == currentOctant)
                {
                    newTree->children.push_back(currentPoint);
                }
                else
                {
                    newTree->children.push_back(CalculateSpaceParameters(newTree, i));
                }
            }

            this->children[octant] = newTree;
        }
    }
}