#include "ColliderComp.h"
#include "GObject.h"
#include "GScene.h"
#include "GPhysics.h"
#include <iostream>
GColliderComp::GColliderComp(GObject *myParentObject, ColliderType col, float radius) : GComponent(myParentObject)
{
    myColliderType = col;
    circleRadius = radius;
}

GColliderComp::GColliderComp(GObject *myParentObject, ColliderType col, ColliderShapeTemplate *shape) : GComponent(myParentObject)
{
    myShape = shape;
    myColliderType = col;
}

bool GColliderComp::CalculateBoundingBox()
{
    //std::cout << "bound" << std::endl;
    if (gobject == nullptr){ return false;}
    gobject->transform.calculateCosAndSine();
    
    if (myColliderType == ColliderType::Polygon)
    {
        float maxX = gobject->transform.local_to_scene(myShape->points[0]).X; 
        float minX = gobject->transform.local_to_scene(myShape->points[0]).X; 
        float maxY = gobject->transform.local_to_scene(myShape->points[0]).Y; 
        float minY = gobject->transform.local_to_scene(myShape->points[0]).Y;
        for (int i = 0; i < myShape->numPoints; i++)
        {
            Vec2D transformedPoint = gobject->transform.local_to_scene(myShape->points[i]);
            if (transformedPoint.X > maxX) maxX = transformedPoint.X;
            if (transformedPoint.Y > maxY) maxY = transformedPoint.Y;
            if (transformedPoint.X < minX) minX = transformedPoint.X;
            if (transformedPoint.Y < minY) minY = transformedPoint.Y;
            
        }
        boundingBoxMax = Vec2D(maxX, maxY);
        boundingBoxMin = Vec2D(minX, minY);
        return true;
    }
    if (myColliderType == ColliderType::Circle)
    {
        boundingBoxMax = Vec2D(gobject->transform.position.X + circleRadius, gobject->transform.position.Y + circleRadius);
        boundingBoxMin = Vec2D(gobject->transform.position.X - circleRadius, gobject->transform.position.Y - circleRadius);
        return true;
    }
    
    return false;
    
}

void GColliderComp::Begin()
{
    gobject->myScene->physics.registerCollider(this);
}

void GColliderComp::Tick(float DeltaTime)
{
    //gobject->myScene->physics.grid.InsertCollider(this);
}
