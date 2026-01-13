#ifndef HEADER_GPhysics
#define HEADER_GPhysics
#include "Vec2D.h"
#include "GTransform.h"
#include "SFML/Graphics.hpp"

class GScene;
class GColliderComp;
class GPhysics
{
public:
    struct Collision{
        Vec2D point;
        Vec2D normal;
        float depth;
        GColliderComp* col1; //col1 is the object whose side is the intersecting axis
        GColliderComp* col2; //col2 is the object whose point is inside the other object
    };

    enum ForceType{
        Force,
        Impulse
    };

    GPhysics(GScene* scene);
    void Tick(float DeltaTime);
    void Begin();

    Collision GetCollisionBetweenObjects(GColliderComp* obj1, GColliderComp* obj2, sf::RenderWindow* window);
    void HandleCollision(Collision col);

    void AddImpulseAtLocation(GObject* const object, Vec2D location , Vec2D force, ForceType ForceType = GPhysics::ForceType::Force);

    private:
    
    GScene* myScene;
    void ApplyVelocities(float DeltaTime);

    

};
#endif