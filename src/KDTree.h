//
// Created by antoine on 24/05/2021.
//

#ifndef SPHERE_KDTREE_H
#define SPHERE_KDTREE_H

#include "Particle.h"
#include "Constraint.h"
#include <iostream>
#include <stdlib.h>
#include <omp.h>

#define X 0
#define Y 1
#define Z 2

class KDTree {

private:
    KDTree *left;
    KDTree *right;
    int axis;
    int maxDepth;
    int minParticles = 2;
    std::vector<Particle*> particles;
    Ra::Core::Vector3 firstPoint;

public:

    KDTree(std::vector<Particle*> p, int axis, int maxDepth);
    KDTree(int axis, int maxDepth);
    void push_back(Particle* p);
    void generate();
    int pushCollisionConstraint(std::vector<Constraint *> *constraints);

};


#endif //SPHERE_KDTREE_H
