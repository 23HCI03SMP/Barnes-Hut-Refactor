#include <iostream>
#include "include/barnes.h"

int main()
{
    Space *space = new Space(Point(0, 0, 0), Point(4, 4, 4));
    // Particle *part1 = new Particle("A", Point(0, 0, 0), Velocity(0, 0, 0), 1, Charge(ELEMENTARYCHARGE, 0));
    // Particle *part2 = new Particle("B", Point(5, 5, 5), Velocity(0, 0, 0), 1, Charge(0, ELEMENTARYCHARGE));
    // // Particle *part3 = new Particle("C", Point(3, 3, 3), Velocity(0, 0, 0), 1, Charge(ELEMENTARYCHARGE, 0));

    // space->insert(part1);
    // space->insert(part2);
    // // space->insert(part3);
    BaseParticle bp = BaseParticle("Proton", 1, Charge(ELEMENTARYCHARGE, 0));
    bp.bField = Field(0, 0, 0);
    bp.eForce = Force(0, 0, 0);

    BaseParticle bp2 = BaseParticle("Electron", 1, Charge(0, ELEMENTARYCHARGE));
    bp2.bField = Field(0, 0, 0);
    bp2.eForce = Force(0, 0, 0);

    std::initializer_list<ParticleInsertionParameters> insertionParameters = {
        ParticleInsertionParameters(bp, 0.5),
        ParticleInsertionParameters(bp2, 0.5)
    };

    // space->generateParticles(20, insertionParameters, 20, Shape::SPHERE, {1.0});
    space->insert(new Particle("A", Point(3, 4, 0), Velocity(0, 0, 0), 1, Charge(ELEMENTARYCHARGE, 0)));
    space->insert(new Particle("B", Point(3.5, 3.5, 0), Velocity(0, 0, 0), 1, Charge(0, ELEMENTARYCHARGE)));

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

    // counts for each particle
    // for (auto p : a)
    // {
    //     std::cout << p->alias << std::endl;
    // }

    // delete part1;
    // delete part2;

    return 0;
}