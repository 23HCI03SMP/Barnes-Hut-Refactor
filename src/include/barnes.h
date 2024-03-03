#include <vector>
#include <string>
#pragma once

// Public variables
constexpr double pi = 3.14159265358979323846;
constexpr double speedOfLight = 3e8; // m/s

constexpr double theta = 0.5;
constexpr double mu0 = 4 * pi * 1e-7; // N/A^2
constexpr double epsilon0 = 1/(mu0 * speedOfLight * speedOfLight); // F/m
constexpr double timeStep = 0.01; // s
constexpr double elementaryCharge = 1.6e-19; // C
constexpr double amu = 1.67e-27; // kg
constexpr double electronMass = 9.11e-31; // kg

// Structs
struct Point
{
    double x, y, z; // m

    Point() {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Velocity
{
    double x, y, z; // m/s

    Velocity() {}
    Velocity(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Acceleration {
    double x, y, z; // m/s^2
    Acceleration(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Distance {
    double x, y, z; // m
    Distance(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Force {
    double x, y, z; // N

    Force() {}
    Force(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Charge
{
    double positive, negative; // C

    Charge() {}
    Charge(double positive, double negative) : positive(positive), negative(negative) {}

    Charge operator+(Charge &other)
    {
        return Charge(this->positive + other.positive, this->negative + other.negative);
    }

    Charge operator+=(Charge &other)
    {
        this->positive += other.positive;
        this->negative += other.negative;
        return *this;
    }
};

struct Field {
    int x, y, z;

    Field() {}
    Field(int x, int y, int z) : x(x), y(y), z(z) {}
};

//Simulation Parameters
const Field externalMagneticField = Field(0, 0, 0); // T
const Field externalElectricField = Field(0, 0, 0); // V/m


// Classes
class Node
{
public:
    Charge charge;

    Node(Charge charge);
    bool isExternalNode();

    virtual ~Node(){};

protected:
    Node(){};
};

enum Shape
{
    SPHERE,
    CYLINDER,
    HOLLOW_CYLINDER
};

class Particle: public Node{
    public:
        std::string alias; //CHANGE TYPE!
        Point pos;
        Velocity velocity;
        Field bField; // this is a field instead of a force because F Michel's Method
        Force eForce;
        double mass;

        Particle(std::string alias, Point pos, Velocity velocity, double mass, Charge charge);
        void updatePosition(double timeStep);
        void calculateBForce(Node *other);
        void calculateEForce(Node *other);
};

class Space : public Node
{
public:
    Point minPoint, maxPoint, centreOfPositveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;

    Space(Point minPoint, Point maxPoint, Charge charge = Charge(0, 0));
    void recalculateCentreOfCharge();
    void insert(Node *node);
    bool find(Point point);
    bool isExternalNode();
    std::vector<Particle *> getChildren();
    std::vector<Particle *> generateParticles(double density, Particle particle, double temperature, Shape shape, ...);

    ~Space();
};

// Utility Functions
void output(Space *space);