#include "ColliderComp.h"
#include "GObject.h"

GColliderComp::GColliderComp(GObject *myParentObject, ColliderType col) : GComponent(myParentObject)
{
    myColliderType = col;
}

GColliderComp::GColliderComp(GObject *myParentObject, ColliderType col, ColliderShapeTemplate *shape) : GComponent(myParentObject)
{
    myShape = shape;
    myColliderType = col;
}

void GColliderComp::CalculateBoundingBox()
{
    if (gobject == nullptr){ return;}
    gobject->transform.calculateCosAndSine();
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
    
}
