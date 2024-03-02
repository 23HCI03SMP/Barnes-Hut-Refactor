#include <iostream>
#include <fstream>
#include <filesystem> // Include the necessary header file for std::filesystem
#include "include/barnes.h"
#include <vector>

void output(Space *space)
{
    std::remove("output.csv");
    std::ofstream file(std::filesystem::current_path().string() + "/output.csv"); // Qualify std::ofstream with std namespace
    file.open("output.csv", std::ios_base::app);

    std::vector<Particle *> particles = space->getChildren(); // @Sunzizhuo add this please

    for (Particle *particle : particles)
    {
        file << particle->pos.x << "," << particle->pos.y << "," << particle->pos.z << std::endl;
    }

    file.close();
}