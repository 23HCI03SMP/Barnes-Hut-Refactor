#include "include/barnes.h"
#include <stdlib.h>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <string>
#include <iostream>

Particle::Particle(std::string alias, Point pos, Velocity velocity, double mass, Charge charge) : Node(charge)
{
    this->alias = alias;
    this->pos = pos;
    this->velocity = velocity;
    this->mass = mass;
    this->bField = bField;
    this->eForce = eForce;
}

void Particle::updatePosition(double timeStep)
{
    // std::cout << "1 " << this->alias << " " << this->pos.x << " " << this->pos.y << " " << this->pos.z << std::endl;
    // Magnetic Force
    // Calculate the velocity of the particle due to the magnetic force
    Point p = Point(this->bField.x * timeStep / mass, this->bField.y * timeStep / mass, this->bField.z * timeStep / mass);

    Eigen::Matrix3d A{
        {0, -p.z, p.y},
        {p.z, 0, -p.x},
        {-p.y, p.x, 0}};

    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d IA = (I + A).inverse();
    Eigen::Vector3d Vprime = IA * (I - A) * Eigen::Vector3d(this->velocity.x, this->velocity.y, this->velocity.z);
    this->velocity = Velocity(Vprime(0), Vprime(1), Vprime(2));

    // Electric Force
    // Calculate the acceleration of the particle due to the electric force
    Acceleration acceleration = Acceleration(this->eForce.x / mass, this->eForce.y / mass, this->eForce.z / mass);
    // Update the velocity of the particle due to the electric force
    this->velocity = Velocity(this->velocity.x + acceleration.x * timeStep, this->velocity.y + acceleration.y * timeStep, this->velocity.z + acceleration.z * timeStep);
    // std::cout << "2 " << this->alias << " " << this->pos.x << " " << this->pos.y << " " << this->pos.z << std::endl;

    // std::cout << this->pos.x << " + " << this->velocity.x << " * " << timeStep << " = " << this->pos.x + this->velocity.x * timeStep << std::endl;
    this->pos = Point(this->pos.x + this->velocity.x * timeStep, this->pos.y + this->velocity.y * timeStep, this->pos.z + this->velocity.z * timeStep);

    // std::cout << "3 " << this->alias << " " << this->pos.x << " " << this->pos.y << " " << this->pos.z << std::endl;
}

Field BiotSavartLaw(Node other, Charge charge, float euclideanDistance, Distance distance, Velocity velocity)
{
    Field bField = Field(((mu0 / (4 * pi)) * ((other.charge.positive - other.charge.negative) / (euclideanDistance * euclideanDistance * euclideanDistance) * (velocity.y * distance.z - velocity.z * distance.y))) + externalMagneticField.x,
                         ((mu0 / (4 * pi)) * ((other.charge.positive - other.charge.negative) / (euclideanDistance * euclideanDistance * euclideanDistance) * (velocity.z * distance.x - velocity.x * distance.z))) + externalMagneticField.y,
                         ((mu0 / (4 * pi)) * ((other.charge.positive - other.charge.negative) / (euclideanDistance * euclideanDistance * euclideanDistance) * (velocity.x * distance.y - velocity.y * distance.x))) + externalMagneticField.z);
    return bField;
}

Force CoulombsLaw(Node other, Charge charge, float euclideanDistance, Distance distance)
{
    Force eForce = Force((1 / (4 * pi * epsilon0)) * (((charge.positive - charge.negative) * (other.charge.positive - other.charge.negative)) / (euclideanDistance * euclideanDistance * euclideanDistance)) * distance.x + (externalElectricField.x * charge.positive),
                         (1 / (4 * pi * epsilon0)) * (((charge.positive - charge.negative) * (other.charge.positive - other.charge.negative)) / (euclideanDistance * euclideanDistance * euclideanDistance)) * distance.y + (externalElectricField.y * charge.positive),
                         (1 / (4 * pi * epsilon0)) * (((charge.positive - charge.negative) * (other.charge.positive - other.charge.negative)) / (euclideanDistance * euclideanDistance * euclideanDistance)) * distance.z + (externalElectricField.z * charge.positive));
    return eForce;
}

void Particle::calculateBForce(Node *other, Point centreOfCharge)
{
    Distance distance = Distance(this->pos.x - centreOfCharge.x, this->pos.y - centreOfCharge.y, this->pos.z - centreOfCharge.z); // Negative = left/below
    float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
    // Biot Savart Law for Point Charges
    Field newbField = BiotSavartLaw(*other, charge, euclideanDistance, distance, this->velocity);
    this->bField = Field(this->bField.x + newbField.x, this->bField.y + newbField.y, this->bField.z + newbField.z);
}

void Particle::calculateEForce(Node *other, Point centreOfCharge)
{
    Distance distance = Distance(this->pos.x - centreOfCharge.x, this->pos.y - centreOfCharge.y, this->pos.z - centreOfCharge.z); // Negative = left/below
    float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
    // Coloumb's Law + External Electric Field
    Force neweForce = CoulombsLaw(*other, charge, euclideanDistance, distance);
    // std::cout << "New eForce: " << neweForce.x << " " << neweForce.y << " " << neweForce.z << std::endl;
    this->eForce = Force(this->eForce.x + neweForce.x, this->eForce.y + neweForce.y, this->eForce.z + neweForce.z);
}

void Particle::simulate(Node *node)
{
    // this->bField = Field(0, 0, 0);
    // this->eForce = Force(0, 0, 0);

    bool isSame = node == this;

    if (node == this) // Compare memory location
    {
        std::cout << "Node is the same as particle. Trying to simulate itself. This is normal" << std::endl;
        return;
    }

    if (node->isExternalNode())
    {
        if (dynamic_cast<Particle *>(node))
        {
            this->calculateBForce(node, dynamic_cast<Particle *>(node)->pos);
            this->calculateEForce(node, dynamic_cast<Particle *>(node)->pos);
        }
    }
    else if (dynamic_cast<Space *>(node))
    {
        Space *space = dynamic_cast<Space *>(node);
        // Calculate s/d
        double s = space->maxPoint.x - space->minPoint.x; // Width of octant

        // Positive CoC
        Distance posChargeDistance = Distance(this->pos.x - space->centreOfPositveCharge.x, this->pos.y - space->centreOfPositveCharge.y, this->pos.z - space->centreOfPositveCharge.z);
        double posD = sqrt(posChargeDistance.x * posChargeDistance.x + posChargeDistance.y * posChargeDistance.y + posChargeDistance.z * posChargeDistance.z);

        // Negative CoC
        Distance negChargeDistance = Distance(this->pos.x - space->centreOfNegativeCharge.x, this->pos.y - space->centreOfNegativeCharge.y, this->pos.z - space->centreOfNegativeCharge.z);
        double negD = sqrt(negChargeDistance.x * negChargeDistance.x + negChargeDistance.y * negChargeDistance.y + negChargeDistance.z * negChargeDistance.z);

        double avgD = (posD + negD) / 2;

        double ratio = s / avgD;

        if (ratio < theta)
        {
            this->calculateBForce(node, space->centreOfPositveCharge);
            this->calculateEForce(node, space->centreOfPositveCharge);

            this->calculateBForce(node, space->centreOfNegativeCharge);
            this->calculateEForce(node, space->centreOfNegativeCharge);
        }
        else
        {
            for (Node *child : space->children)
            {
                this->simulate(child);
            }
        }
    }
    else
    {
        throw "Node is not a particle or a space?";
    }
}