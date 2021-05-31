// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>
#include <Engine/Scene/Component.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Utils/Chronometer.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Data/DrawPrimitives.hpp>
#include <QTimer>
#include <Gui/Viewer/Viewer.hpp>
#include "Particle.h"
#include "ParticleManager.h"
#include <iostream>
#include <fstream>

Ra::Engine::Scene::Entity *createPlane(std::string name, Ra::Core::Vector3 size, Ra::Core::Vector3 position, Ra::Gui::BaseApplication *app){

    auto plane = Ra::Core::Geometry::makeBox(size);
    plane.addAttrib(
            "in_color",
            Ra::Core::Vector4Array {plane.vertices().size(),
                                    Ra::Core::Utils::ColorBase<Scalar>(Scalar(237.f/255.f), Scalar(187.f/255.f), Scalar(153.f/255.f))} );

    auto planeEntity = app->m_engine->getEntityManager()->createEntity(name + " entity");

    auto planeMesh = new Ra::Engine::Scene::TriangleMeshComponent (name + " mesh", planeEntity, std::move(plane), nullptr);
    planeMesh->initialize();
    auto geometrySystem = app->m_engine->getSystem("GeometrySystem");

    geometrySystem->addComponent(planeEntity, planeMesh);

    auto planeTransform = planeEntity->getTransform();

    planeTransform.translate(position);
    //planeTransform.rotate(rotation);

    planeEntity->setTransform(planeTransform);

    return planeEntity;
}

class particleAnimation : public Ra::Engine::Scene::System
{
    ParticleManager *p;
    Ra::Gui::BaseApplication* app;
    Ra::Core::Utils::Clock clock;
    Ra::Core::Utils::Clock::time_point spawn, time;
    float dt;
    int nb;

public:

    inline particleAnimation(ParticleManager *s, Ra::Gui::BaseApplication* a) : p(s), app(a){
        clock = Ra::Core::Utils::Clock();
        spawn = clock.now();
        dt = 0;
    }

    virtual void generateTasks(Ra::Core::TaskQueue* q,
                               const Ra::Engine::FrameInfo&) override {

        q->registerTask( new Ra::Core::FunctionTask(
                [this](){

                    if( (clock.now() - spawn) / std::chrono::milliseconds(1) >= 200) {
                        spawn = clock.now();
                        Particle *par = new Particle(std::to_string(p->particleNb()), 0.3f,Ra::Core::Vector3(-4.f, 5.f, 0.f), *app);
                        par->setVelocity(Ra::Core::Vector3(.2f, 0.f, 0.f));
                        p->addParticle(par);
                        app->m_mainWindow->getViewer()->getRenderer()->buildAllRenderTechniques();
                    }


                    p->update(1.f);
                },
                "particle update"
                ));
    }

};



int main(int argc, char **argv){

    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize( Ra::Gui::SimpleWindowFactory {} );

    //create all the walls
    Ra::Engine::Scene::Entity *left = createPlane("left", Ra::Core::Vector3(0.1, 5, 5), Ra::Core::Vector3(-5.1, 5, 0), &app);
    Ra::Core::Aabb leftCollision = left->computeAabb();
    leftCollision.translate(Ra::Core::Vector3(-5.1, 5, 0));

    Ra::Engine::Scene::Entity *back = createPlane("back", Ra::Core::Vector3(5, 5, 0.1), Ra::Core::Vector3(0, 5, -5.1), &app);
    Ra::Core::Aabb backCollision = back->computeAabb();
    backCollision.translate(Ra::Core::Vector3(0, 5, -5.1));

    Ra::Engine::Scene::Entity *r = createPlane("right", Ra::Core::Vector3(0.1, 5, 5), Ra::Core::Vector3 (5.1, 5, 0), &app);
    Ra::Core::Aabb rc = r->computeAabb();
    rc.translate(Ra::Core::Vector3(5.1, 5, 0));

    Ra::Engine::Scene::Entity *t = createPlane("bottom", Ra::Core::Vector3(5, 0.1, 5), Ra::Core::Vector3 (0, 0.1f, 0), &app);
    Ra::Core::Aabb tc = t->computeAabb();
    tc.translate(Ra::Core::Vector3(0, 0.1f, 0));

    Ra::Engine::Scene::Entity *b= createPlane("top", Ra::Core::Vector3(5, 0.1, 5), Ra::Core::Vector3 (0, 9.9, 0), &app);
    Ra::Core::Aabb bc = b->computeAabb();
    bc.translate(Ra::Core::Vector3(0, 9.9, 0));

    Ra::Core::Aabb frontCollision = back->computeAabb();
    frontCollision.translate(Ra::Core::Vector3(0, 5, 5.1));

    //create a ParticleManager and add link all the obstacles (the walls) to it
    ParticleManager *pm = new ParticleManager(&app);
    pm->addObstacle(&leftCollision);
    pm->addObstacle(&rc);
    pm->addObstacle(&bc);
    pm->addObstacle(&tc);
    pm->addObstacle(&backCollision);
    pm->addObstacle(&frontCollision);

    particleAnimation* anim = new particleAnimation(pm, &app);

    app.m_engine->registerSystem("AnimationSystem", anim);

    app.m_mainWindow->prepareDisplay();
    app.m_mainWindow->getViewer()->getRenderer()->enableDebugDraw(true);

    return app.exec();

}

