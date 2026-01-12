#include "GPhysics.h"
#include "GComponent.h"
#include "GScene.h"
#include "GObject.h"
#include "ColliderComp.h"
#include <iostream>
#include <cmath>
GPhysics::GPhysics(GScene* scene)
{
    myScene = scene;
}

void GPhysics::Tick(float DeltaTime)
{
    
    ApplyVelocities(DeltaTime);
    for (auto& obj : myScene->sceneObjects)
    {
        if (!obj->getPhysicsEnabled()) continue;
        //obj->velocity = obj->velocity + Vec2D(0,-19.8)*DeltaTime;
        //std::cout << obj->velocity.Y << std::endl;
    }
}

void GPhysics::Begin()
{

}

void GPhysics::GetCollisionBetweenObjects(GColliderComp *obj1, GColliderComp *obj2)
{
    
}

void GPhysics::AddImpulseAtLocation(GObject *const object, Vec2D location ,Vec2D force, ForceType ForceType)
{
    Vec2D acc = force * (1/object->mass);
    Vec2D diff = location - object->transform.myScenePosition();
    float torque = Vec2D::CrossProduct(diff, force);
    object->angularVelocity += torque/object->momentOfInertia;
    std::cout << torque << std::endl;
    object->velocity = object->velocity+acc;
}

void GPhysics::ApplyVelocities(float DeltaTime)
{
    for (auto& obj : myScene->sceneObjects)
    {
        if (!obj->getPhysicsEnabled()) continue;
        obj->transform.position = obj->transform.position + obj->velocity*DeltaTime;
        obj->transform.rotation = obj->transform.rotation + obj->angularVelocity*DeltaTime;
    }
}
