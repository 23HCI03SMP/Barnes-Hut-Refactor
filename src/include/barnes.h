// Public variables
double pi = 3.14159265358979323846;
extern double speedOfLight = 3e8;

extern double theta;
extern double mu0 = 4 * pi * 1e-7;
extern double epsilon0 = mu0 * speedOfLight * speedOfLight;
extern double timeStep = 1e-6;
extern double primaryCharge = 1.6e-19;
extern double amu = 1.67e-27;
extern double electronMass = 9.11e-31;

// Structs
struct Point {
    double x, y, z;
};

struct Velocity {
    double x, y, z;
};

struct Force {
    double x, y, z;
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
        Force bForce;
        Force eForce;
        double mass;

        void Particle();
        void updatePosition(double timeStep);
        void calculateBForce(Node root);
        void calculateEForce(Node root);
        void calculateForce(Node root);
};