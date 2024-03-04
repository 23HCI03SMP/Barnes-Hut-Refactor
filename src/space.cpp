#include "include/barnes.h"
#include <iostream>
#include <numeric>
#include <vector>
#include <cstdarg>
#include <cmath>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

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
    this->centreOfPositveCharge = Point();
    this->centreOfNegativeCharge = Point();
}

Space::~Space()
{
    for (Node *child : this->getChildren())
    {
        delete child;
    }
}

// density refers to the number of particles per unit volume
std::vector<Particle *> Space::generateParticles(double density, std::initializer_list<ParticleInsertionParameters> insertionParticles, double temperature, Shape shape, std::initializer_list<double> dimensions)
{
    std::vector<Particle *> particles;

    gsl_rng *rng = gsl_rng_alloc(gsl_rng_default);
    gsl_rng_set(rng, time(NULL));

    switch (shape)
    {
    case Shape::SPHERE:
    {
        if (dimensions.size() != 1)
            throw std::runtime_error("Invalid number of dimensions (expected 1) for sphere shape.");

        // if the ratios don't add up to 1
        if (std::accumulate(insertionParticles.begin(), insertionParticles.end(), 0.0, [](double sum, ParticleInsertionParameters particle) { return sum + particle.ratio; }) != 1)
            throw std::runtime_error("Ratios don't add up to 1");

        double radius = *dimensions.begin();

        for (ParticleInsertionParameters insertionParameterParticle : insertionParticles)
        {
            double n = std::ceil(density * (4.0 / 3.0) * PI * radius * radius * radius * insertionParameterParticle.ratio);

            for (int i = 0; i < n; i++)
            {
                double x = gsl_ran_flat(rng, -radius, radius) + (this->maxPoint.x + this->minPoint.x) / 2;
                double y = gsl_ran_flat(rng, -radius, radius) + (this->maxPoint.y + this->minPoint.y) / 2;
                double z = gsl_ran_flat(rng, -radius, radius) + (this->maxPoint.z + this->minPoint.z) / 2;

                double vx = gsl_ran_gaussian(rng, sqrt((K_BOLTZMANN * temperature) / insertionParameterParticle.particle.mass));
                double vy = gsl_ran_gaussian(rng, sqrt((K_BOLTZMANN * temperature) / insertionParameterParticle.particle.mass));
                double vz = gsl_ran_gaussian(rng, sqrt((K_BOLTZMANN * temperature) / insertionParameterParticle.particle.mass));

                Particle *particle = new Particle(insertionParameterParticle.particle.alias, Point(x, y, z), Velocity(vx, vy, vz), insertionParameterParticle.particle.mass, insertionParameterParticle.particle.charge);
                particle->bField = insertionParameterParticle.particle.bField;
                particle->eForce = insertionParameterParticle.particle.eForce;

                this->insert(particle);
                particles.push_back(particle);
            }
        }

        break;
    }
    case Shape::CYLINDER:
    {
        // double radius = va_arg(args, double);
        // double height = va_arg(args, double);

        break;
    }
    case Shape::HOLLOW_CYLINDER:
    {
        // double innerRadius = ;
        // double outerRadius = ;
        // double height = va_arg(args, double);

        break;
    }
    default:
    {
        throw std::runtime_error("Invalid shape");
    }
    }

    return particles;
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

                Space *space = dynamic_cast<Space *>(child);

                // if (std::isnan(space->centreOfPositveCharge.x) || std::isnan(space->centreOfNegativeCharge.x)) continue;

                if (!std::isnan(space->centreOfPositveCharge.x))
                {
                    xPosPositiveChargeSum += space->centreOfPositveCharge.x * child->charge.positive;
                    yPosPositiveChargeSum += space->centreOfPositveCharge.y * child->charge.positive;
                    zPosPositiveChargeSum += space->centreOfPositveCharge.z * child->charge.positive;
                }

                if (!std::isnan(space->centreOfNegativeCharge.x))
                {
                    xPosNegativeChargeSum += space->centreOfNegativeCharge.x * child->charge.negative;
                    yPosNegativeChargeSum += space->centreOfNegativeCharge.y * child->charge.negative;
                    zPosNegativeChargeSum += space->centreOfNegativeCharge.z * child->charge.negative;
                }
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
    throw std::runtime_error("Not implemented");
}

Space *CalculateSpaceParameters(Space *baseSpace, int octant)
{
    double xMax = (*baseSpace).maxPoint.x;
    double xMin = (*baseSpace).minPoint.x;
    double yMax = (*baseSpace).maxPoint.y;
    double yMin = (*baseSpace).minPoint.y;
    double zMax = (*baseSpace).maxPoint.z;
    double zMin = (*baseSpace).minPoint.z;


    switch (octant)
    {
    case o1: // top left back
        return new Space(Point((*baseSpace).minPoint.x, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z / 2), Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y, (*baseSpace).maxPoint.z), Charge());
    case o2: // top right back
        return new Space(Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z / 2), (*baseSpace).maxPoint, Charge());
    case o3: // top right front
        return new Space(Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y / 2, (*baseSpace).minPoint.z), Point((*baseSpace).maxPoint.x, (*baseSpace).maxPoint.y, (*baseSpace).maxPoint.z / 2), Charge());
    case o4: // top left front
        return new Space(Point((*baseSpace).minPoint.x, (*baseSpace).maxPoint.y / 2, (*baseSpace).minPoint.z), Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z / 2), Charge());
    case o5: // bottom left back
        return new Space(Point((*baseSpace).minPoint.x, (*baseSpace).minPoint.y, (*baseSpace).maxPoint.z / 2), Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z), Charge());
    case o6: // bottom right back
        return new Space(Point((*baseSpace).maxPoint.x / 2, (*baseSpace).minPoint.y, (*baseSpace).maxPoint.z / 2), Point((*baseSpace).maxPoint.x, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z), Charge());
    case o7: // bottom right front
        return new Space(Point((*baseSpace).maxPoint.x / 2, (*baseSpace).minPoint.y, (*baseSpace).minPoint.z), Point((*baseSpace).maxPoint.x, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z / 2), Charge());
    case o8: // bottom left front
        return new Space((*baseSpace).minPoint, Point((*baseSpace).maxPoint.x / 2, (*baseSpace).maxPoint.y / 2, (*baseSpace).maxPoint.z / 2), Charge());
    default:
        throw std::runtime_error("This shouldn't even happen (Octant shouldn't be anything other than o1 to o8)");
    }
}

int GetOctant(Particle *particle, Space *baseSpace)
{
    int octant;
    double xMid = ((*baseSpace).maxPoint.x + (*baseSpace).minPoint.x) / 2;
    double yMid = ((*baseSpace).maxPoint.y + (*baseSpace).minPoint.y) / 2;
    double zMid = ((*baseSpace).maxPoint.z + (*baseSpace).minPoint.z) / 2;

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
        // int octant = GetOctant(dynamic_cast<Particle *>(node), this);
        // Space *space = CalculateSpaceParameters(this, octant);

        // // if the space doesn't have any octants, create them
        // if (this->children.size() == 0)
        // {
        //     for (int i = o1; i <= o8; i++)
        //     {
        //         space->children.push_back(CalculateSpaceParameters(this, i));
        //     }

        //     this->children = space->children;
        // }

        if (this->children.size() == 0)
        {
            for (int i = o1; i <= o8; i++)
            {
                this->children.push_back(CalculateSpaceParameters(this, i));
            }
        }

        int octant = GetOctant(dynamic_cast<Particle *>(node), this);

        // decide which octant the node belongs to
        // int octant = GetOctant(dynamic_cast<Particle *>(node), this);
        Node *octantNode = this->children[octant]; // get the octant node

        if (dynamic_cast<Space *>(octantNode)) // if the octant is a space
        {
            if (dynamic_cast<Space *>(octantNode)->isExternalNode()) // if the octant has no chidren, simply replace it with the node
            {
                // delete octantNode;
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
                throw std::runtime_error("Particle already exists. Boom! Fusion reaction!");
            }

            // Create a new space and insert the current particle and the new particle into the new space, along with other octants
            Space *newTree = CalculateSpaceParameters(this, octant);

            int insertOctant = GetOctant(insertParticle, newTree);
            int currentOctant = GetOctant(currentPoint, newTree);
            
            if (insertOctant == currentOctant)
            {
                newTree->insert(insertParticle);
                newTree->insert(currentPoint);
            }
            else
            {
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
            }

            this->children[octant] = newTree;
        }
    } 
    else
    {
        throw std::runtime_error("Invalid node type");
    }
}