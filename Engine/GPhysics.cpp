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
        obj->velocity = obj->velocity + Vec2D(0,-9.8)*DeltaTime;
        //std::cout << obj->velocity.Y << std::endl;
    }
    Damping(DeltaTime);
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
            float multiplier = (col.col2->gobject->bounce + col.col1->gobject->bounce)/2 + 1;
            Vec2D vel_at_hit_point = col.col2->gobject->velocity_at_point(col.point);

            Vec2D r = col.point - col.col2->gobject->transform.myScenePosition();

            col.col2->gobject->transform.position = col.col2->gobject->transform.position + col.normal*col.depth; 
            float vel_normal_scalar = Vec2D::DotProduct(col.normal, vel_at_hit_point);
            Vec2D normalVel = col.normal*vel_normal_scalar;

            //applying coefficient of restitution formula
            float rxn = Vec2D::CrossProduct(r, col.normal);
            float j = multiplier * vel_normal_scalar;
            float invMass = (1.f/col.col2->gobject->mass) + (rxn*rxn)/col.col2->gobject->momentOfInertia;
            j /= invMass;
            AddImpulseAtLocation(col.col2->gobject, col.point, -col.normal*j);

            //friction
            float avg_friction = (col.col1->gobject->friction + col.col2->gobject->friction)/2;
            Vec2D tangent = Vec2D::GetPerpendicular(col.normal);

            float rxt = Vec2D::CrossProduct(r, tangent);
            float invMassTang = (1.f/col.col2->gobject->mass) + (rxt*rxt)/col.col2->gobject->momentOfInertia;
            float jt = Vec2D::DotProduct(vel_at_hit_point, tangent)/invMassTang;
            float maxFric = avg_friction*j;
            if (maxFric < 0) maxFric = -maxFric;
            jt = std::clamp(jt, -maxFric, maxFric);
            // float vel_along_tangent = Vec2D::DotProduct(tangent, vel_at_hit_point);
            // if (vel_along_tangent > 0)
            // {
            //     tangent = -tangent;
            //     vel_along_tangent = - vel_along_tangent;
            // }
            AddImpulseAtLocation(col.col2->gobject, col.point, tangent*(-jt));//*normalVel.magnitude()*avg_friction);
        }
    }
    else
    {
        if (!col.col2->gobject->getPhysicsEnabled())
        {
            float multiplier = (col.col2->gobject->bounce + col.col1->gobject->bounce)/2 + 1;
            Vec2D vel_at_hit_point = col.col1->gobject->velocity_at_point(col.point);

            Vec2D r = col.point - col.col1->gobject->transform.myScenePosition();

            col.col1->gobject->transform.position = col.col1->gobject->transform.position - col.normal*col.depth;   
            Vec2D normalVel = col.normal*Vec2D::DotProduct(col.normal, vel_at_hit_point);
            float vel_normal_scalar = Vec2D::DotProduct(col.normal, vel_at_hit_point);

            //coeff of restitution formula j = -(1+e)*v
            float rxn = Vec2D::CrossProduct(r, col.normal);
            float j = multiplier * vel_normal_scalar;
            float invMass = (1.f/col.col1->gobject->mass) + (rxn*rxn)/col.col1->gobject->momentOfInertia; //because impulse
            j /= invMass;
            AddImpulseAtLocation(col.col1->gobject, col.point, -col.normal*j);

            //implimenting friction
            float avg_friction = (col.col1->gobject->friction + col.col2->gobject->friction)/2;
            Vec2D tangent = Vec2D::GetPerpendicular(col.normal);
            float vel_along_tangent = Vec2D::DotProduct(tangent, vel_at_hit_point);

            float rxt = Vec2D::CrossProduct(r, tangent);
            float invMassTang = (1.f/col.col1->gobject->mass) + (rxt*rxt)/col.col1->gobject->momentOfInertia;
            float jt = Vec2D::DotProduct(vel_at_hit_point, tangent)/invMassTang;
            float maxFric = avg_friction*j;
            if (maxFric < 0) maxFric = -maxFric;
            jt = std::clamp(jt, -maxFric, maxFric);

            // if (vel_along_tangent > 0)
            // {
            //     tangent = -tangent;
            //     vel_along_tangent = - vel_along_tangent;
            // }
            AddImpulseAtLocation(col.col1->gobject, col.point, tangent*jt);//normalVel.magnitude()*avg_friction);

            //std::cout << normalVel.magnitude() << std::endl;
        }

        //both objects are rigid bodies
        else 
        {
            //normal vector is from B to A - from 1 to 2
            //1 is the object whose side got intersected
            //a point on 2 got inside object 1
            //normal is pointing in the direction of 1 to 2 to push 2 out of 1

            float e = (col.col2->gobject->bounce + col.col1->gobject->bounce)/2;
            col.col1->gobject->transform.position = col.col1->gobject->transform.position - col.normal*col.depth/2;
            col.col2->gobject->transform.position = col.col2->gobject->transform.position + col.normal*col.depth/2; 
            
            float j = -(1+e);  

            Vec2D v1p = col.col1->gobject->velocity_at_point(col.point); //B
            Vec2D v2p = col.col2->gobject->velocity_at_point(col.point); //A

            Vec2D R2p = col.point - col.col2->gobject->transform.position; //Ra
            Vec2D R1p = col.point - col.col1->gobject->transform.position; //Rb
            Vec2D v21 = v2p-v1p; //from B to A

            float r2xn = Vec2D::CrossProduct(R2p, col.normal);
            float r1xn = Vec2D::CrossProduct(R1p, col.normal);
            j *= Vec2D::DotProduct(col.normal, v21);

            float invMass = (1.f/col.col1->gobject->mass) + (1.f/col.col2->gobject->mass);
            invMass += (r1xn*r1xn)/col.col1->gobject->momentOfInertia + (r2xn*r2xn)/col.col2->gobject->momentOfInertia;

            j /= invMass;

            AddImpulseAtLocation(col.col1->gobject, col.point, -col.normal*j);
            AddImpulseAtLocation(col.col2->gobject, col.point, col.normal*j);

            float avg_friction = (col.col1->gobject->friction + col.col2->gobject->friction)/2;
            Vec2D tangent = Vec2D::GetPerpendicular(col.normal);
            
            float o1_tangent_vel = Vec2D::DotProduct(tangent, v1p);
            float o2_tangent_vel = Vec2D::DotProduct(tangent, v2p);

            float r2xt = Vec2D::CrossProduct(R2p, tangent);
            float r1xt = Vec2D::CrossProduct(R1p, tangent);

            float invMassTang = (1.f/col.col1->gobject->mass) + (1.f/col.col2->gobject->mass);
            invMassTang += (r1xt*r1xt)/col.col1->gobject->momentOfInertia + (r2xt*r2xt)/col.col2->gobject->momentOfInertia;

            float jt = Vec2D::DotProduct(v21, tangent)/invMassTang;

            
            float maxFric = avg_friction*j;
            if (maxFric < 0) maxFric = -maxFric;

            jt = std::clamp(jt, -maxFric, maxFric);

            float relative_tan_vel = o1_tangent_vel - o2_tangent_vel;
            // if (relative_tan_vel < 0)
            // {
            //     relative_tan_vel = -relative_tan_vel;
            //     tangent = -tangent;
            // }

            AddImpulseAtLocation(col.col1->gobject, col.point, tangent*jt);
            AddImpulseAtLocation(col.col2->gobject, col.point, -tangent*jt);


        }
    }


    
}

void GPhysics::AddImpulseAtLocation(GObject *const object, Vec2D location ,Vec2D force)
{
    Vec2D acc = force/object->mass;
    Vec2D diff = location - object->transform.myScenePosition();
    float torque = Vec2D::CrossProduct(diff, force);
    object->angularVelocity += torque/object->momentOfInertia;
    std::cout << acc.magnitude() << std::endl;
    object->velocity = object->velocity+acc;
}

void GPhysics::AddVelocityAtLocation(GObject *const object, Vec2D location ,Vec2D velocity)
{
    //Vec2D acc = force * (1/object->mass);
    Vec2D diff = location - object->transform.myScenePosition();
    float angular = Vec2D::CrossProduct(diff, velocity);
    object->angularVelocity += angular;
    //std::cout << torque << std::endl;
    object->velocity = object->velocity+velocity;
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

void GPhysics::Damping(float DeltaTime)
{
    float linearDamp = 0.1f;
    float angularDamp = 0.05f;
    for (auto& obj : myScene->sceneObjects)
    {
        if (!obj->getPhysicsEnabled()) continue;
        obj->velocity = obj->velocity*(1-linearDamp*DeltaTime);
        obj->angularVelocity = obj->angularVelocity*(1-angularDamp*DeltaTime);
    }
}
