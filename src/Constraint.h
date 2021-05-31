//
// Created by antoine on 20/05/2021.
//

#ifndef SPHERE_CONSTRAINT_H
#define SPHERE_CONSTRAINT_H

#include "Core/Types.hpp"
#include "Particle.h"

class Constraint {

public:
    virtual float eval() = 0;
    virtual Ra::Core::Vector3 gradient(Particle* p);
    virtual void updateVel(){}
    virtual void solve(){}
    virtual ~Constraint(){}
};

//collision between 2 Particles constraint
class ConstraintPP : public Constraint {

private:
    Particle* p1;
    Particle* p2;

public:
    ConstraintPP(Particle *pp1, Particle *pp2);
    float eval();
    void solve();
    void updateVel();


};

//collisions between a particle and an Aabb constraint
class ConstraintPO : public Constraint {

private:
    Particle* p;
    Ra::Core::Aabb* o;

public:
    ConstraintPO(Particle *pp, Ra::Core::Aabb* oo);
    Ra::Core::Vector3 closestPoint(Ra::Core::Vector3 p);
    void solve();
    float eval();
    void updateVel();
};



#endif //SPHERE_CONSTRAINT_H
