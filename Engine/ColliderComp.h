#ifndef HEADER_GColl
#define HEADER_GColl

#include "GComponent.h"

/*
Collider class. derived from GComponent. can be added as a component to an object by doing
auto* coll = CreateComponent<GColliderComp>(newObj, GColliderComp::ColliderType::Polygon, &shapetemplate1);

If the collider is not a circle a shape template must be assigned.
*/
 
class GObject;
struct ColliderShapeTemplate{
    int numPoints;
    Vec2D *points;
};

class GColliderComp : public GComponent{
    public:
    enum ColliderType{
        Circle,
        Polygon
    };

    GColliderComp(GObject* myParentObject, ColliderType col, float radius);
    GColliderComp(GObject* myParentObject, ColliderType col, ColliderShapeTemplate *shape);

    bool CalculateBoundingBox();
    void Begin() override;
    void Tick(float DeltaTime) override;

    ColliderShapeTemplate* myShape;
    float circleRadius;
    ColliderType myColliderType;

    Vec2D boundingBoxMax;
    Vec2D boundingBoxMin;
    
};
#endif