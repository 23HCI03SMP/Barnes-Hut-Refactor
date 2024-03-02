// Public variables
double pi = 3.14159265358979323846;
extern double speedOfLight = 3e8;

extern double theta;
extern double mu0 = 4 * pi * 1e-7; // N/A^2
extern double epsilon0 = mu0 * speedOfLight * speedOfLight; // F/m
extern double timeStep = 1e-6; // s
extern double primaryCharge = 1.6e-19; // C
extern double amu = 1.67e-27; // kg
extern double electronMass = 9.11e-31; // kg

// Structs
struct Point {
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Velocity {
    double x, y, z;
    Velocity(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct ForceVector {
    double x, y, z, magnitude;
    ForceVector(double x, double y, double z, double magnitude) : x(x), y(y), z(z), magnitude(magnitude) {}
};


// Classes
class Node {
    public:
        int positiveCharge, negativeCharge;
        Point posChargeCentre, negChargeCentre;

        void Node();
        void recalculateCentreOfCharge();
        void insert(Node node);
        bool find(Point point);
        bool isExternalNode();
};

class Particle: public Node{
    public:
        Point pos;
        Velocity velocity;
        ForceVector bForce;
        ForceVector eForce;
        double mass;

        void Particle();
        void updatePosition(double timeStep);
        void calculateBForce(Node root);
        void calculateEForce(Node root);
        void calculateForce(Node root);
};