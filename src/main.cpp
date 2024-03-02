#include <iostream>
#include "include/barnes.h"

int main() {
    Particle *part1 = new Particle("A", Point(-3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(elementaryCharge, 0));
    Particle *part2 = new Particle("B", Point(3e-11, 0, 0), Velocity(0, 0, 0), 1, Charge(0, elementaryCharge));

    part1->calculateEForce(part2);
    part1->calculateBForce(part2);

    part2->calculateEForce(part1);
    part2->calculateBForce(part1);

    part1->updatePosition(timeStep);
    part2->updatePosition(timeStep);

    Particle particles[2] = {*part1, *part2};
    output(particles);

    delete part1;
    delete part2;

    return 0;
}