#include <vector>

// Public variables
double pi = 3.14159265358979323846;
extern double speedOfLight = 3e8; // m/s

extern double theta;
extern double mu0 = 4 * pi * 1e-7;                          // N/A^2
extern double epsilon0 = mu0 * speedOfLight * speedOfLight; // F/m
extern double timeStep = 1e-6;                              // s
extern double primaryCharge = 1.6e-19;                      // C
extern double amu = 1.67e-27;                               // kg
extern double electronMass = 9.11e-31;                      // kg

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

struct Force
{
    double x, y, z; // N

    Force() {}
    Force(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Charge
{
    double positive, negative; // C

    Charge() {}
    Charge(double positive, double negative) : positive(positive), negative(negative) {}
};

// Classes
class Node
{
public:
    double mass;
    Charge charge;

    Node(double mass, Charge charge);

    virtual ~Node(){};

protected:
    Node(){};
};

class Space : public Node
{
public:
    Point minPoint, maxPoint, centreOfPositveCharge, centreOfNegativeCharge;
    std::vector<Node *> children;

    Space(Point minPoint, Point maxPoint, double mass, Charge charge) : Node(mass, charge){};
    void recalculateCentreOfCharge(Node *node);
    void insert(Node node);
    bool find(Point point);
    bool isExternalNode();
};

class Particle : public Node
{
public:
    std::string alias;
    Point position;
    Velocity velocity;
    Force bForce, eForce;

    Particle(std::string alias, Point position, Velocity velocity, Force bForce, Force eForce, double mass, Charge charge) : Node(mass, charge){};
};