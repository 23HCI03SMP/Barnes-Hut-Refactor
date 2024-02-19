#include <vector>
#include <string>

struct Point
{
    float x, y, z;

    Point(): x(-1), y(-1), z(-1) {}
    Point(float x, float y, float z): x(x), y(y), z(z) {}
};

// class Octree 
// {
//     float mass;
//     float positiveCharge, negativeCharge;

//     float magneticFieldX = 0, magneticFieldY = 0, magneticFieldZ = 0;
//     float electricFieldX = 0, electricFieldY = 0, electricFieldZ = 0;

//     double ke = 0, pe = 0;
// };

// class Node : public Octree 
// {

// };

// class Octant : public Octree 
// {
//     double vx, vy, vz;
// };

struct Octree
{
    Point *point;
    Point *minPoint, *maxPoint;
    Point *positiveCoc, *negativeCoc;

    float mass = 0;
    double vx = 0, vy = 0, vz = 0;
    std::vector<Octree*> children;

    float magneticFieldX = 0, magneticFieldY = 0, magneticFieldZ = 0;
    float electricFieldX = 0, electricFieldY = 0, electricFieldZ = 0;

    float positiveCharge = 0, negativeCharge = 0;
    double ke = 0, pe = 0;

    std::string alias;

    Octree(std::string alias, float x, float y, float z, double vx, double vy, double vz, float mass, float charge);
    Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    void insert(std::string alias, float x, float y, float z, double vx, double vy, double vz, float mass, float charge);
    void insert(Octree *octree);
    void find(float x, float y, float z);
};