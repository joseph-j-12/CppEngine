#ifndef HEADER_GSPRING
#define HEADER_GSPRING

#include "Vec2D.h"
#include "GTransform.h"
#include "GComponent.h"
class GObject;
/*
A Component is attached to GObject. Multiple components can be attached.
The Begin function will be called at start after all the constructors everywhere has initialized
Tick is called every frame if enableComponentTick is true
Different types of components can be created from this component class
*/

class GSpringComponent :  public GComponent{
    public:
    bool enableComponentTick;
    GObject* otherObject;

    GSpringComponent(GObject *myParentObject, GObject *attachedTo) : GComponent(myParentObject)
    {
        gobject = myParentObject;
        otherObject = attachedTo;
    }

    Vec2D myAttachedPos; //this object local space
    Vec2D otherAttachedPos; //other object local space

    float spring_constant;
    float mean_length;
    Vec2D attachDirection; //this object local space
    bool constrainedAlongAxis = false;


    void Begin() override;
    virtual void Tick(float deltaTime) override;

    

};
#endif