#include <vector>
#include <string>
#include <limits>
#pragma once

// Public variables
constexpr double PI = 3.14159265358979323846;
constexpr double c = 3e8; // m/s

constexpr double theta = 0.5;
constexpr double mu0 = 4 * PI * 1e-7;          // N/A^2
constexpr double epsilon0 = 1 / (mu0 * c * c); // F/m
constexpr double timeStep = 0.01;              // s
constexpr double ELEMENTARYCHARGE = 1.6e-19;   // C
constexpr double AMU = 1.67e-27;               // kg
constexpr double M_ELECTRON = 9.11e-31;        // kg
constexpr double K_BOLTZMANN = 1.38e-23;       // J/K

// Structs
struct Point
{
    double x, y, z; // m

    Point()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Velocity
{
    double x, y, z; // m/s

    Velocity()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }
    Velocity(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Acceleration
{
    double x, y, z; // m/s^2
    Acceleration(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Distance
{
    double x, y, z; // m
    Distance(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Force
{
    double x, y, z; // N

    Force()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }
    Force(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Charge
{
    double positive, negative; // C

    Charge() 
    {
        positive = std::numeric_limits<double>::quiet_NaN();
        negative = std::numeric_limits<double>::quiet_NaN();
    }
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

struct Field
{
    double x, y, z;

    Field()
    {
        x = std::numeric_limits<double>::quiet_NaN();
        y = std::numeric_limits<double>::quiet_NaN();
        z = std::numeric_limits<double>::quiet_NaN();
    }
    Field(int x, int y, int z) : x(x), y(y), z(z) {}
};

// Simulation Parameters
const Field externalMagneticField = Field(0, 0, 0); // T
const Field externalElectricField = Field(0, 0, 0); // V/m

// Classes
class Node
{
public:
    Charge charge;

    Node(Charge charge);
    void insert(Node node);
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

class BaseParticle : public Node
{
public:
    std::string alias;
    Field bField; // this is a field instead of a force because F Michel's Method
    Force eForce;
    double mass;

    BaseParticle(std::string alias, double mass, Charge charge);
};

class Particle : public BaseParticle
{
public:
    // std::string alias;
    Point pos;
    Velocity velocity;
    // Field bField; // this is a field instead of a force because F Michel's Method
    // Force eForce;
    // double mass;

    Particle(std::string alias, Point pos, Velocity velocity, double mass, Charge charge);
    void updatePosition(double timeStep);
    void calculateBForce(Node *other);
    void calculateEForce(Node *other);
};

struct ParticleInsertionParameters
{
    BaseParticle particle;
    double ratio;

    ParticleInsertionParameters(BaseParticle particle, double ratio) : particle(particle), ratio(ratio) {}
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
    std::vector<Particle *> generateParticles(double density, std::initializer_list<ParticleInsertionParameters> insertionParticles, double temperature, Shape shape, std::initializer_list<double> dimensions);

    ~Space();
};

// Utility Functions
void output(Space *space);