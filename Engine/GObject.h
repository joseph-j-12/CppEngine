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
    Vec2D velocity;
    float angularVelocity;
    float mass;
    float momentOfInertia;
    //all the attached components

    GObject()
    {
        physicsEnabled = false;
        transform.gobject = this;
        angularVelocity = 0.f;
        mass = 1;
        momentOfInertia = 25;
    }

    void Begin();

    void Tick(float DeltaTime);

    virtual ~GObject() = default;

    template<typename T, typename... Args> T* CreateComponent(Args&&... args)
    {
        auto newComp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = newComp.get();
        myComponents.push_back(std::move(newComp));

        return ptr;
    }

    bool getPhysicsEnabled() {return physicsEnabled;}
    void setPhysicsEnabled(bool en) {physicsEnabled = en;}
    
    std::vector<std::unique_ptr<GComponent>> myComponents; 
    
    private:
    bool physicsEnabled;
    bool colliderEnabled;
};
#endif