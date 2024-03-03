#include <iostream>
#include "include/barnes.h"

int main() {
    Space *space = new Space(Point(0, 0, 0), Point(5, 5, 5));
    Particle *part1 = new Particle("A", Point(0, 0, 0), Velocity(0, 0, 0), 1, Charge(ELEMENTARYCHARGE, 0));
    Particle *part2 = new Particle("B", Point(5, 5, 5), Velocity(0, 0, 0), 1, Charge(0, ELEMENTARYCHARGE));
    Particle *part3 = new Particle("C", Point(3, 3, 3), Velocity(0, 0, 0), 1, Charge(ELEMENTARYCHARGE, 0));

    space->insert(part1);
    space->insert(part2);
    space->insert(part3);
    space->recalculateCentreOfCharge();

    // part1->calculateEForce(space);
    // part1->calculateBForce(space);

    // part2->calculateEForce(space);
    // part2->calculateBForce(space);

    // part1->updatePosition(timeStep);
    // part2->updatePosition(timeStep);

    // Particle particles[2] = {*part1, *part2};
    // output(space);

    auto a = space->getChildren();

    // delete part1;
    // delete part2;

    return 0;
}