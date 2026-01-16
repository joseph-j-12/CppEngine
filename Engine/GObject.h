#ifndef HEADER_GOBJ
#define HEADER_GOBJ

#include "Vec2D.h"
#include "GTransform.h"
#include "GComponent.h"
#include <vector>
#include <memory>
/*
An object that will exist in a scene
Every object will have a transform and the physicsEnabled flag.
*/
class GObject{
    public:
    GTransform transform;
    Vec2D velocity; //world space velocity
    float angularVelocity;


    //physics collision paramters
    float mass;
    float momentOfInertia;
    float bounce; //coefficient of restitution
    float friction;

    GObject()
    {
        physicsEnabled = false;
        transform.gobject = this;
        angularVelocity = 0.f;
        mass = 300;
        momentOfInertia = 30000;
        bounce = 0.3f;
        friction = 0.2f;
        
    }

    void Begin();

    void Tick(float DeltaTime);

    virtual ~GObject() = default;

    template<typename T, typename... Args> T* CreateComponent(Args&&... args)
    {
        auto newComp = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* ptr = newComp.get();
        myComponents.push_back(std::move(newComp));

        return ptr;
    }

    bool getPhysicsEnabled() {return physicsEnabled;}
    void setPhysicsEnabled(bool en) {physicsEnabled = en;}
    
    std::vector<std::unique_ptr<GComponent>> myComponents; 
    
    Vec2D velocity_at_point(Vec2D worldPos)
    {
        Vec2D vel = velocity;
        Vec2D dir = worldPos - transform.local_to_scene(Vec2D(0,0));
        //float length = dir.magnitude();
        
        Vec2D angular = Vec2D::CrossProductZWithVector(angularVelocity, dir);

        return (vel+angular);
    }

    private:
    bool physicsEnabled;
    bool colliderEnabled;
};
#endif