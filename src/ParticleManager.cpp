//
// Created by antoine on 28/04/2021.
//

#include "ParticleManager.h"


ParticleManager::ParticleManager(Ra::Gui::BaseApplication *app){
    ParticleManager::app = app;
}


void ParticleManager::addParticle(Particle *p) {
    particles.push_back(p);
}


void ParticleManager::addObstacle(Ra::Core::Aabb *o) {
    obstacles.push_back(o);
}


void ParticleManager::update(float dt) {

    //external forces are being apply on the particles
    for(auto p : particles){
        p->velocity = p->velocity + ( dt * gravity );
        p->hyPos = p->position + ( dt * p->velocity );
    }

    genCollConstraints();

    for(int t = 0; t < 1; t++){
        for(auto c : constraints){
            c->solve();
        }
    }

    //positions update
    for(auto p: particles){
        //p->velocity = (p->hyPos - p->position)/dt;
        p->setPosition(p->hyPos);
    }

    //velocities update
    for(auto c: constraints){
        c->updateVel();
        delete c;
    }

    constraints.clear();
}


void ParticleManager::genCollConstraints() {
    //particle - obstacles collisions
    for(auto p:particles){
        for(auto o:obstacles){
            Constraint* c = new ConstraintPO(p, o);
            if(c->eval() < 0){
                constraints.push_back(c);
            }else{
                delete c;
            }
        }
    }

    // particle - particle collisions

    /* previous method
    for(int i = 0; i < particles.size(); i++){
        for(int j = i+1; j < particles.size(); j++){
            auto pi = particles[i];
            auto pj = particles[j];
            Constraint* c = new ConstraintPP(pi , pj);
            if(c->eval() < 0) {
                constraints.push_back(c);
            }else{
                delete c;
            }

        }
    }*/

    //KDtree method
    KDTree* kdTree = new KDTree(particles, 0, 32);
    kdTree->generate();
    kdTree->pushCollisionConstraint(&constraints);

}


void ParticleManager::createParticles(int number) {
    srand (time(NULL));
    for(int i=0; i<number; i++){
        float x = (static_cast <float> (rand()) / RAND_MAX);
        float y = (static_cast <float> (rand()) / RAND_MAX);
        float z = (static_cast <float> (rand()) / RAND_MAX);

        Particle* p = new Particle("p "+std::to_string(i), .2f, Ra::Core::Vector3(-0.5 + x, 4.5+y, -0.5 + z), *app);
        p->setVelocity(Ra::Core::Vector3(-0.5 + x, -0.5 + y, -0.5 + z));
        addParticle(p);
    }
}

int ParticleManager::particleNb() {
    return particles.size();
}

void ParticleManager::deleteParticles(){
    for(auto p: particles){
        app->m_engine->getEntityManager()->removeEntity(p->getEntity());
    }
    particles.clear();
}



