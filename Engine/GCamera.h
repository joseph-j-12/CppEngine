#ifndef HEADER_GCAMERA
#define HEADER_GCAMERA

#include "Vec2D.h"
#include "GObject.h"
// class GObject;
/*
Camera class
Manages the scroll values of the camera which is to be subtracted from coordinates 
of each point before converting to screen coordinates.

It also supports object tracking. If not assigned (nullptr) it wont track anything
*/
class GCamera{
    public :
    Vec2D position;
    Vec2D zoom;

    Vec2D screenSize;

    GObject* trackingTarget;
    Vec2D trackingSpeed;
    GScene* scene;

    void Tick(float deltaTime)
    {
        if (trackingTarget)
        {
            if (trackingSpeed.X <= 0.001f && trackingSpeed.Y <= 0.001f)
            {
                position = trackingTarget->transform.myScenePosition();
            }
        }
    }

    Vec2D convertScreenToWorld(Vec2D screenPos)
    {
        return Vec2D((screenPos.X-screenSize.X*0.5f)/zoom.X + position.X,-(screenPos.Y-screenSize.Y*0.5f)/zoom.Y + position.Y);
    }

    Vec2D convertWorldToScreen(Vec2D worldPos)
    {
        return Vec2D(zoom.X*(worldPos.X-position.X)+screenSize.X*0.5f, zoom.Y*(-worldPos.Y+position.Y)+screenSize.Y*0.5f);
    }
};
#endif