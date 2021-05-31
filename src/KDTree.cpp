//
// Created by antoine on 24/05/2021.
//

#include "KDTree.h"

KDTree::KDTree(std::vector<Particle *> p, int axis, int maxDepth) {
    left = nullptr;
    right = nullptr;
    particles = p;
    KDTree::maxDepth = maxDepth;
    KDTree::axis = axis;
}

KDTree::KDTree(int axis, int maxDepth) {
    left = nullptr;
    right = nullptr;
    KDTree::maxDepth = maxDepth;
    KDTree::axis = axis;
}

void KDTree::push_back(Particle *p) {
    particles.push_back(p);
}


bool axisX(Particle *a, Particle *b) {
    return a->getHyPos()[0] >= b->getHyPos()[0];
}


bool axisY(Particle* a, Particle* b){
    return a->getHyPos()[1] >= b->getHyPos()[1];
}

bool axisZ(Particle* a, Particle* b){
    return a->getHyPos()[2] >= b->getHyPos()[2];
}

#include  <random>
#include  <iterator>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

void KDTree::generate() {
    /*switch(axis){
        case 0:
            std::sort(particles.begin(), particles.end(), axisX);
            break;
        case 1:
            std::sort(particles.begin(), particles.end(), axisY);
            break;
        case 2:
            std::sort(particles.begin(), particles.end(), axisZ);
            break;
        default:
            break;
    }*/

    if(maxDepth > 0 && particles.size() > minParticles) {
        Particle *middle = particles[particles.size() - 1];//*select_randomly(particles.begin(), particles.end());
        float pos = middle->getHyPos()[axis] + middle->getRadius();
        Particle *p;
        left = new KDTree(axis + 1 % 3, maxDepth - 1);
        right = new KDTree(axis + 1 % 3, maxDepth - 1);
        for (int i = 0; i < particles.size(); i++) {
            p = particles[i];

            if (p->getHyPos()[axis] + p->getRadius() > pos)
                right->push_back(p);

            if (p->getHyPos()[axis] - p->getRadius() <= pos)
                left->push_back(p);

        }

        left->generate();
        right->generate();

    }

}

int KDTree::pushCollisionConstraint(std::vector<Constraint *> *constraints) {
    int c = 0;
    if(maxDepth == 0 || particles.size() <= minParticles){
        c+= particles.size();
        for (int i = 0; i < particles.size(); i++) {
            for (int j = i + 1; j < particles.size(); j++) {
                auto pi = particles[i];
                auto pj = particles[j];
                Constraint *c = new ConstraintPP(pi, pj);
                if (c->eval() < 0) {
                    constraints->push_back(c);
                } else {
                    delete c;
                }
            }
        }
    }else{
        c += left->pushCollisionConstraint(constraints);
        c += right->pushCollisionConstraint(constraints);
    }
    delete this;
    return c;
}

