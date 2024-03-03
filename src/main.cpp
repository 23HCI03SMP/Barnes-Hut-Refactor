#include <iostream>
#include "include/barnes.h"

int main()
{
    // Corners of the space
    Point p1 = Point(-5, -5, -5);
    Point p2 = Point(5, 5, 5);

    Space *space = new Space(p1, p2);
    Particle *part1 = new Particle("A", Point(-1e-10, 0, 0), Velocity(0, 0, 0), 1, Charge(elementaryCharge, 0));
    Particle *part2 = new Particle("B", Point(1e-10, 0, 0), Velocity(0, 0, 0), 1, Charge(0, elementaryCharge));

    space->insert(part1);
    space->insert(part2);

    space->recalculateCentreOfCharge();

    // Particle *part1 = new Particle("A", Point(-3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(elementaryCharge, 0));
    // Particle *part2 = new Particle("B", Point(3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(0, elementaryCharge));

    std::remove("output.csv");

    for (int i = 0; i < iterations; i++)
    {
        auto a = space->getChildren();
        Space *newSpace = new Space(p1, p2);

        std::vector<Particle *> particles;
        for (Particle *i : a)
        {
            i->simulate(space);
            particles.push_back(i);
        }
        for (Particle *i : particles)
        {
            // std::cout << "1 " << i->alias << " " << i->pos.x << " " << i->pos.y << " " << i->pos.z << std::endl;
            // Update space with new positions
            i->updatePosition(timeStep);
            // std::cout << "2 " << i->alias << " " << i->pos.x << " " << i->pos.y << " " << i->pos.z << std::endl;
            // Insert the particle into the new space
            Particle *particle = new Particle(i->alias, i->pos, i->velocity, i->mass, i->charge);
            // std::cout << "3 " << particle->alias << " " << i->pos.x << " " << i->pos.y << " " << i->pos.z << std::endl;

            newSpace->insert(particle);
        }
        output(space);
        delete space;
        space = newSpace;
    }

    delete space;

    return 0;
}