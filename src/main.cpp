#include <iostream>
#include "include/barnes.h"

int main() {
    Space *space = new Space(Point(-5, -5, -5), Point(5, 5, 5));
    Particle *part1 = new Particle("A", Point(-3, 0, 0), Velocity(0, 0, 0), 1, Charge(elementaryCharge, 0));
    Particle *part2 = new Particle("B", Point(3, 0, 0), Velocity(0, 0, 0), 1, Charge(0, elementaryCharge));

    space->insert(part1);
    space->insert(part2);
    space->recalculateCentreOfCharge();

    // Particle *part1 = new Particle("A", Point(-3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(elementaryCharge, 0));
    // Particle *part2 = new Particle("B", Point(3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(0, elementaryCharge));

    part1->calculateEForce(space);
    part1->calculateBForce(space);

    part2->calculateEForce(space);
    part2->calculateBForce(space);

    part1->updatePosition(timeStep);
    part2->updatePosition(timeStep);

    std::cout << "Particle 1: " << part1->pos.x << ", " << part1->pos.y << ", " << part1->pos.z << std::endl;
    std::cout << "Particle 2: " << part2->pos.x << ", " << part2->pos.y << ", " << part2->pos.z << std::endl;

    auto a = space->getChildren();

    // delete part1;
    // delete part2;

    return 0;
}