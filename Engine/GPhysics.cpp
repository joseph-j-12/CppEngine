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


//return the collision between two objects.
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
                
                //if not overlaping on any one axis, then the objects are not colliding
                if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) {
                    col.depth = -1;
                    col.colliding = false;
                    return col;
                }
                
                //calculate overlap and normal of overlap
                //the collision normal will be the axis of least overlap
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
        }
    }

    //to find the point of collision. 
    //the point is considered the one that has gotten most inside the other shape
    //this is found by taking the projection of each point onto the normal of collision
    //the point with least value will be the one inside the other (least as in could be negative as well)

    //assume that origin is on the side thru which normal goes then points inside the shape will have negative projection
    col.colliding = true;
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
    if (!col.col1->gobject->getPhysicsEnabled())
    {
        if (!col.col2->gobject->getPhysicsEnabled())
        {
            return;
        }
        else
        {
            float multiplier = col.col2->gobject->bounce * col.col1->gobject->bounce;
            col.col2->gobject->transform.position = col.col2->gobject->transform.position + col.normal*col.depth;   
            Vec2D normalVel = col.normal*Vec2D::DotProduct(col.normal, col.col2->gobject->velocity_at_point(col.point));
            AddImpulseAtLocation(col.col2->gobject, col.point, -normalVel*multiplier, ForceType::Impulse);
        }
    }
    else
    {
        if (!col.col2->gobject->getPhysicsEnabled())
        {
            float multiplier = col.col2->gobject->bounce * col.col1->gobject->bounce;
            col.col1->gobject->transform.position = col.col1->gobject->transform.position - col.normal*col.depth;   
            Vec2D normalVel = col.normal*Vec2D::DotProduct(col.normal, col.col1->gobject->velocity_at_point(col.point));
            AddImpulseAtLocation(col.col1->gobject, col.point, -normalVel*multiplier, ForceType::Impulse);
        }
    }


    
}

void GPhysics::AddImpulseAtLocation(GObject *const object, Vec2D location ,Vec2D force, ForceType ForceType)
{
    Vec2D acc = force * (1/object->mass);
    Vec2D diff = location - object->transform.myScenePosition();
    float torque = Vec2D::CrossProduct(diff, force);
    object->angularVelocity += torque/object->momentOfInertia;
    //std::cout << torque << std::endl;
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
