#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>
#include <Core/Geometry/RayCast.hpp>
#include <Core/Types.hpp>
#include <iostream>


#ifndef SPHERE_PARTICLE_H
#define SPHERE_PARTICLE_H


class Particle {

    friend class ParticleManager;
    friend class Constraint;

protected:

    //physical part
    float radius;
    std::string name;
    Ra::Core::Vector3 velocity;
    Ra::Core::Vector3 hyPos;
    Ra::Core::Vector3 position;


private:

    //Radium engine part
    Ra::Core::Geometry::TriangleMesh sphere;
    Ra::Engine::Scene::SurfaceMeshComponent<Ra::Core::Geometry::TriangleMesh> *sphereMesh;
    Ra::Engine::Scene::Entity *entity;
    Ra::Core::Transform *transform;

public:

    Particle(std::string name, float radius, Ra::Core::Vector3 position, Ra::Gui::BaseApplication &app);
    ~Particle();
    void update();

    Ra::Core::Transform* getTransform();
    Ra::Core::Vector3 getVelocity();
    float getRadius();
    Ra::Core::Vector3 getPosition();
    std::string getName();
    Ra::Core::Vector3 getHyPos();
    Ra::Engine::Scene::Entity* getEntity();

    void setHyPos(Ra::Core::Vector3 p);
    void setPosition(Ra::Core::Vector3 p);
    void setVelocity(Ra::Core::Vector3 v);


};


#endif //SPHERE_PARTICLE_H
