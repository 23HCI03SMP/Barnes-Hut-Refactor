# include "include/barnes.h"
# include <stdlib.h>
# include <cmath>
# include <eigen3/Eigen/Dense>
# include <string>

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
    // Electric Force
    // Calculate the acceleration of the particle due to the electric force
    Acceleration acceleration = Acceleration(eForce.x / mass, eForce.y / mass, eForce.z / mass);
    // Update the velocity of the particle due to the electric force
    velocity = Velocity(velocity.x + acceleration.x * timeStep, velocity.y + acceleration.y * timeStep, velocity.z + acceleration.z * timeStep);

    // Magnetic Force
    // Calculate the velocity of the particle due to the magnetic force
    Point p = Point(bField.x * timeStep / mass, bField.y * timeStep / mass, bField.z * timeStep / mass);

    Eigen::Matrix3d A {
        {0, -p.y, p.y},
        {p.z, 0, -p.x},
        {-p.y, p.x, 0}
    };

    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d IA = (I + A).inverse();
    Eigen::Vector3d Vprime = IA * (I-A) * Eigen::Vector3d(velocity.x, velocity.y, velocity.z);
    velocity = Velocity(velocity.x + Vprime(0), velocity.y + Vprime(1), velocity.z + Vprime(2));

    pos = Point(pos.x + velocity.x * timeStep, pos.y + velocity.y * timeStep, pos.z + velocity.z * timeStep);
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

void Particle::calculateBForce(Node *other)
{
    bool isspace = dynamic_cast<Space*>(other) ? true : false; // Check if the other node is a space or a particle

    if (isspace)
    {
        Point chargeCentre = dynamic_cast<Space*>(other)->centreOfNegativeCharge;
        Distance distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Biot Savart Law for Point Charges
        bField = BiotSavartLaw(*other, charge, euclideanDistance, distance, velocity);
        
        chargeCentre = dynamic_cast<Space*>(other)->centreOfPositveCharge;
        distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Biot Savart Law for Point Charges
        Field newBField = BiotSavartLaw(*other, charge, euclideanDistance, distance, velocity);
        bField = Field(bField.x + newBField.x, bField.y + newBField.y, bField.z + newBField.z);
    }
    else
    {
        Point chargeCentre = dynamic_cast<Particle*>(other)->pos;
        Distance distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Biot Savart Law for Point Charges
        bField = BiotSavartLaw(*other, charge, euclideanDistance, distance, velocity);
    }
}

void Particle::calculateEForce(Node *other)
{
    bool isspace = dynamic_cast<Space*>(other) ? true : false; // Check if the other node is a space or a particle

    if (isspace)
    {
        // Neg COC
        Point chargeCentre = dynamic_cast<Space*>(other)->centreOfNegativeCharge;
        Distance distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Coloumb's Law + External Electric Field
        eForce = CoulombsLaw(*other, charge, euclideanDistance, distance);
        
        // Pos COC
        chargeCentre = dynamic_cast<Space*>(other)->centreOfPositveCharge;
        distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Coloumb's Law + External Electric Field
        Force newEForce = CoulombsLaw(*other, charge, euclideanDistance, distance);
        eForce = Force(eForce.x + newEForce.x, eForce.y + newEForce.y, eForce.z + newEForce.z);
    }
    else
    {
        Point chargeCentre = dynamic_cast<Particle*>(other)->pos;
        Distance distance = Distance(pos.x - chargeCentre.x, pos.y - chargeCentre.y, pos.z - chargeCentre.z); // Negative = left/below
        float euclideanDistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        //Coloumb's Law + External Electric Field
        eForce = CoulombsLaw(*other, charge, euclideanDistance, distance);
    }
}