#include "GPhysics.h"
#include "GComponent.h"
#include "GScene.h"
#include "GObject.h"
#include "ColliderComp.h"
#include <iostream>
#include <cmath>
#include <array>
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
        obj->velocity = obj->velocity + Vec2D(0,-19.8)*DeltaTime;
        //std::cout << obj->velocity.Y << std::endl;
    }
}

void GPhysics::Begin()
{

}

GPhysics::Collision GPhysics::GetCollisionBetweenObjects(GColliderComp *obj1, GColliderComp *obj2, sf::RenderWindow* window)
{
    //Separating axis theorem
    GColliderComp* o1 = obj1;  
    GColliderComp* o2 = obj2;
    GColliderComp* object_ther_than_axis;
    GColliderComp* object_that_is_axis;
    o1->gobject->transform.calculateCosAndSine();
    o2->gobject->transform.calculateCosAndSine();
    float minOverlap = INFINITY;
    Vec2D overlapNormal;
    Vec2D point;
    Collision col;
    for (int i = 0; i < 2; i++)
    {
        if (i == 1)
        {
            o1 = obj2;  
            o2 = obj1;
        }

        if (o1->myColliderType == GColliderComp::ColliderType::Polygon)
        {
            for (int j = 0; j < o1->myShape->numPoints; j++)
            {
                int k = (j+1);
                if (k>=o1->myShape->numPoints) k = 0;

                Vec2D axis = Vec2D::GetPerpendicular(o1->gobject->transform.local_to_scene(o1->myShape->points[k])-o1->gobject->transform.local_to_scene(o1->myShape->points[j])).getNormal();
                

                //finding projection of 1st shape
                float min_r1 = INFINITY; float max_r1 = -INFINITY;
                for (int p = 0; p < o1->myShape->numPoints; p++)
                {
                    float proj = Vec2D::DotProduct(axis, o1->gobject->transform.local_to_scene(o1->myShape->points[p]));
                    min_r1 = std::min(min_r1, proj);
                    max_r1 = std::max(max_r1, proj);
                }
                
                //2nd shape
                float min_r2 = INFINITY; float max_r2 = -INFINITY;
                for (int p = 0; p < o2->myShape->numPoints; p++)
                {
                    float proj = Vec2D::DotProduct(axis, o2->gobject->transform.local_to_scene(o2->myShape->points[p]));
                    min_r2 = std::min(min_r2, proj);
                    max_r2 = std::max(max_r2, proj);
                }
                //std::cout << max_r2 << "-" << min_r2 << "-" << max_r1 << "-" << min_r1 << std::endl;
                if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) {
                    col.depth = -1;
                    return col;
                }
                
                //calculate overlap and normal of overlap
                float ov = (std::min(max_r1, max_r2) - std::max(min_r1, min_r2));
                if (ov < minOverlap)
                {
                    minOverlap = ov;
                    overlapNormal = axis;
                    object_ther_than_axis = o2;
                    object_that_is_axis = o1;
                    if (Vec2D::DotProduct(overlapNormal, (o2->gobject->transform.local_to_scene(Vec2D())-o1->gobject->transform.local_to_scene(Vec2D()))) < 0)
                    {
                        overlapNormal = -overlapNormal;
                    }
                }
            }
            //std::cout << "-----------" << std::endl;
        }
    }
    //std::cout << "-----------" << std::endl;
    col.depth = minOverlap;
    col.normal = overlapNormal;
    float proj = INFINITY;
    for (int i = 0; i < object_ther_than_axis->myShape->numPoints; i++)
    {
        float t = Vec2D::DotProduct(col.normal, object_ther_than_axis->gobject->transform.local_to_scene(object_ther_than_axis->myShape->points[i]));
        if (t<proj) {
            proj = t; 
            col.point = object_ther_than_axis->gobject->transform.local_to_scene(object_ther_than_axis->myShape->points[i]);
        }
    }
    col.col1 = object_that_is_axis;
    col.col2 = object_ther_than_axis; //other_than_axis. i made a type now i dont want to change
    return col;
}

void GPhysics::HandleCollision(Collision col)
{
    float multiplier = col.depth;
    if (!col.col1->gobject->getPhysicsEnabled())
    {
        if (!col.col2->gobject->getPhysicsEnabled())
        {
            return;
        }
        multiplier *= 0.5f;
    }
    if(col.col1->gobject->getPhysicsEnabled())
    {
        col.col1->gobject->transform.position = col.col1->gobject->transform.position - col.normal*multiplier;
        //col.col1->gobject->velocity = col.col1->gobject->velocity - col.normal*25;
        AddImpulseAtLocation(col.col1->gobject, col.point, col.normal*(-25), ForceType::Impulse);
    } 
    if (col.col2->gobject->getPhysicsEnabled())
    {
        col.col2->gobject->transform.position = col.col2->gobject->transform.position + col.normal*multiplier;   
        col.col2->gobject->velocity = col.col2->gobject->velocity + col.normal*25;
        AddImpulseAtLocation(col.col2->gobject, col.point, col.normal*25, ForceType::Impulse);
    }
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
