//
// Created by antoine on 28/04/2021.
//

#ifndef SPHERE_PARTICLEMANAGER_H
#define SPHERE_PARTICLEMANAGER_H

#include <iostream>
#include <Core/Types.hpp>
#include "Particle.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Constraint.h"
#include "KDTree.h"

class ParticleManager {

private:

    Ra::Core::Vector3 gravity = Ra::Core::Vector3(0.f, -.001f, 0.f);
    std::vector<Particle *> particles;
    std::vector<Ra::Core::Aabb *> obstacles;
    std::vector<Constraint *> constraints;
    Ra::Gui::BaseApplication *app;

public:

    ParticleManager(Ra::Gui::BaseApplication *app);
    void addParticle(Particle *p);
    void addObstacle(Ra::Core::Aabb *o);
    void createParticles(int number);
    int particleNb();
    void deleteParticles();
    void update(float dt);
    void genCollConstraints();

};


#endif //SPHERE_PARTICLEMANAGER_H
