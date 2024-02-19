#include "include/barnes.h"

#define o1 0
#define o2 1
#define o3 2
#define o4 3
#define o5 4
#define o6 5
#define o7 6
#define o8 7

Octree::Octree(std::string alias, float x, float y, float z, double vx, double vy, double vz, float mass, float charge)
{
    this->alias = alias;
    this->point = new Point(x, y, z);
    this->mass = mass;
    this->vx = vx;
    this->vy = vy;
    this->vz = vz;
    
    if (charge > 0)
    {
        this->positiveCharge = charge;
    }
    else if (charge < 0)
    {
        this->negativeCharge = charge;
    }
    else
    {
        this->positiveCharge = 0;
        this->negativeCharge = 0;
    }
}

// Base octree
Octree::Octree(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    if (minX > maxX || minY > maxY || minZ > maxZ)
    {
        throw "Invalid dimensions";
    }

    this->point = nullptr;
    this->minPoint = new Point(minX, minY, minZ);
    this->maxPoint = new Point(maxX, maxY, maxZ);

    this->children.assign(8, nullptr);
    for (int i = 0; i < 8; i++)
    {
        this->children[i] = nullptr;
    }
}

void Octree::insert(Octree* octree) 
{
    // check if point already exists


    if (octree->point->x < this->minPoint->x || octree->point->x > this->maxPoint->x || octree->point->y < this->minPoint->y || octree->point->y > this->maxPoint->y || octree->point->z < this->minPoint->z || octree->point->z > this->maxPoint->z) return;

    float midX = (this->minPoint->x + this->maxPoint->x) / 2;
    float midY = (this->minPoint->y + this->maxPoint->y) / 2;
    float midZ = (this->minPoint->z + this->maxPoint->z) / 2;
    float pos;

    if (octree->point->x <= midX)
    {
        if (octree->point->y <= midY)
        {
            if (octree->point->z <= midZ)
            {
                pos = o1;
            }
            else
            {
                pos = o5;
            }
        }
        else
        {
            if (octree->point->z <= midZ)
            {
                pos = o3;
            }
            else
            {
                pos = o7;
            }
        }
    }
    else
    {
        if (octree->point->y <= midY)
        {
            if (octree->point->z <= midZ)
            {
                pos = o2;
            }
            else
            {
                pos = o6;
            }
        }
        else
        {
            if (octree->point->z <= midZ)
            {
                pos = o4;
            }
            else
            {
                pos = o8;
            }
        }
    }

    if (this->children[pos] == nullptr)
    {
        // this->children[pos] = octree;
    }
    else
    {
        this->children[pos]->insert(octree);
    }
}

void Octree::insert(std::string alias, float x, float y, float z, double vx, double vy, double vz, float mass, float charge) 
{
    Octree *octree = new Octree(alias, x, y, z, vx, vy, vz, mass, charge);
    this->insert(octree);

    delete octree;
} 