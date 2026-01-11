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

    //all the attached components
    std::vector<std::unique_ptr<GComponent>> myComponents; 

    GObject()
    {
        physicsEnabled = false;
    }

    void Begin();

    void Tick(float DeltaTime);

    ~GObject() = default;

    private:
    bool physicsEnabled;
};
#endif