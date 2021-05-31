//
// Created by antoine on 20/05/2021.
//
#include "Constraint.h"
#include "Particle.h"

//this function is from the work of M2 student: http://fsi-dpt-info.univ-tlse3.fr/master-igai/2015-g1/
Ra::Core::Vector3 Constraint::gradient(Particle* p) {

    float epsilon = 0.001f;
    float baseC, x, y, z;
    baseC = eval();

    p->hyPos.x() += epsilon;
    x = eval() - baseC;
    p->hyPos.x() -= epsilon;

    p->hyPos.y() += epsilon;
    y = eval() - baseC;
    p->hyPos.y() -= epsilon;

    p->hyPos.z() += epsilon;
    z = eval() - baseC;
    p->hyPos.z() -= epsilon;

    Ra::Core::Vector3 gradient = Ra::Core::Vector3(x, y, z);
    return gradient/epsilon;

}


ConstraintPP::ConstraintPP(Particle *pp1, Particle *pp2){
    p1 = pp1;
    p2 = pp2;
}

float ConstraintPP::eval() {
    return (p1->getHyPos() - p2->getHyPos()).norm() - (p1->getRadius() + p2->getRadius());
}

void ConstraintPP::updateVel() {
    auto po = p1->getPosition() - p2->getPosition();
    auto c = (p1->getVelocity() - p2->getVelocity()).dot(po);
    c /= po.squaredNorm();
    auto delta = po * c;

    p1->setVelocity((p1->getVelocity() - delta) * 0.9); //some artificial energy loss is added
    p2->setVelocity((p2->getVelocity() + delta) * 0.9);
}

void ConstraintPP::solve() {

    auto sum = gradient(p1).squaredNorm() + gradient(p2).squaredNorm();
    auto s = (sum==0? 0 : eval()/sum);
    auto dp1 = -s * gradient(p1);
    auto dp2 = -s * gradient(p2);

    p1->setHyPos(p1->getHyPos() + dp1);
    p2->setHyPos(p2->getHyPos() + dp2);

}

ConstraintPO::ConstraintPO(Particle *pp, Ra::Core::Aabb* oo){
    p = pp;
    o = oo;
}

Ra::Core::Vector3 ConstraintPO::closestPoint(Ra::Core::Vector3 p){
    float x, y, z;

    //closest point
    if(p.x() > o->max().x()){
        x = o->max().x();
    }else if(p.x() < o->min().x()){
        x = o->min().x();
    }else{
        x = p.x();
    }
    if(p.y() > o->max().y()){
        y = o->max().y();
    }else if(p.y() < o->min().y()){
        y = o->min().y();
    }else{
        y = p.y();
    }
    if(p.z() > o->max().z()){
        z = o->max().z();
    }else if(p.z() < o->min().z()){
        z = o->min().z();
    }else{
        z = p.z();
    }

    return Ra::Core::Vector3(x, y, z);
}

float ConstraintPO::eval() {
    return (closestPoint(p->getHyPos()) - p->getHyPos()).norm() - p->getRadius();
}

void ConstraintPO::updateVel() {
    auto normal = (p->getPosition() - closestPoint(p->getPosition())).normalized();
    auto vel = p->getVelocity();
    vel = vel - (2 * vel.dot(normal)) * normal;
    p->setVelocity(vel * 0.9); //some artificial energy loss is added
}

void ConstraintPO::solve() {
    auto s = gradient(p).squaredNorm();
    s = s==0? 0 : eval()/s;
    auto lambda = -s * gradient(p);
    p->setHyPos(p->getHyPos() + lambda);
}