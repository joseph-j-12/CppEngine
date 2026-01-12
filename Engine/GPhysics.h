#ifndef HEADER_GPhysics
#define HEADER_GPhysics
#include "Vec2D.h"
#include "GTransform.h"

class GScene;
class GPhysics
{
public:
    struct Collision{
        Vec2D point;
        Vec2D normal;
        Vec2D tangent;
    };

    enum ForceType{
        Force,
        Impulse
    };

    GPhysics(GScene* scene);
    void Tick(float DeltaTime);
    void Begin();

    void GetCollisionBetweenObjects(GColliderComp* obj1, GColliderComp* obj2);

    void AddImpulseAtLocation(GObject* const object, Vec2D location , Vec2D force, ForceType ForceType = GPhysics::ForceType::Force);

    private:
    GScene* myScene;
    void ApplyVelocities(float DeltaTime);

    

};
#endif