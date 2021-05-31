#include "Particle.h"

Particle::Particle(std::string name, float radius, Ra::Core::Vector3 pos, Ra::Gui::BaseApplication &app):
name(name), radius(radius)
{

    //particles component and stuff initialisation
    sphere = Ra::Core::Geometry::makeCapsule(0.f, radius);
    sphere.addAttrib(
            "in_color",
            Ra::Core::Vector4Array {sphere.vertices().size(),
                                    Ra::Core::Utils::ColorBase<Scalar>(Scalar(252.f/255.f), Scalar(90.f/255.f), Scalar(90.f/255.f))} );
    entity = app.m_engine->getEntityManager()->createEntity(name + " entity");
    auto sphereMesh = new Ra::Engine::Scene::TriangleMeshComponent(name + " mesh", entity, std::move(sphere), nullptr);
    Particle::sphereMesh = sphereMesh;
    app.m_engine->getSystem("GeometrySystem")->addComponent(entity, sphereMesh);

    //transform initialisation
    transform = new Ra::Core::Transform();
    transform->setIdentity();
    position = pos;
    transform->translate(position);
    entity->setTransform(*transform);

    entity->swapTransformBuffers();
    velocity = Ra::Core::Vector3(0, 0, 0);

}

void Particle::setVelocity(Ra::Core::Vector3 v){
    velocity = v;
}

void Particle::setPosition(Ra::Core::Vector3 p) {
    transform->setIdentity();
    position = p;
    transform->translate(position);
    entity->setTransform(*transform);
}

void Particle::setHyPos(Ra::Core::Vector3 p) {
    hyPos = p;
}


float Particle::getRadius() {
    return radius;
}

Ra::Core::Vector3 Particle::getPosition() {
    return position;
}

Ra::Core::Vector3 Particle::getHyPos() {
    return hyPos;
}

Ra::Core::Vector3 Particle::getVelocity() {
    return velocity;
}

Ra::Engine::Scene::Entity* Particle::getEntity(){
    return entity;
}

Particle::~Particle() {
    sphere.clear();
}
