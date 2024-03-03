#include <iostream>
#include <fstream>
#include <filesystem> // Include the necessary header file for std::filesystem
#include "include/barnes.h"
#include <vector>

void output(Space *space) // Add the default argument for mode
{
    std::ofstream file(std::filesystem::current_path().string() + "/output.csv", std::ios_base::app); // Qualify std::ofstream with std namespace

    auto particles = space->getChildren(); // @Sunzizhuo add this please

    for (Particle *particle : particles)
    {
        file << particle->alias << "," << particle->pos.x << "," << particle->pos.y << "," << particle->pos.z << "," << particle->velocity.x << "," << particle->velocity.y << "," << particle->velocity.z << "," 
        << particle->eForce.x << "," << particle->eForce.y << "," << particle->eForce.z << "," << particle->bField.x << "," << particle->bField.y << "," << particle->bField.z << std::endl;
        //file << particle->pos.x << "," << particle->pos.y << "," << particle->pos.z << "," << particle->alias << std::endl;
    }
    file << std::endl;

    file.close();
}