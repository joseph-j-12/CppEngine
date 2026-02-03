#include "GSpringComponent.h"
#include "GObject.h"
#include "GScene.h"

void GSpringComponent::Begin()
{
}

void GSpringComponent::Tick(float deltaTime)
{
    if (otherObject)
    {
        if (gobject)
        {
            Vec2D myPointWorld = gobject->transform.local_to_scene(myAttachedPos);
            Vec2D otherPointWorld = otherObject->transform.local_to_scene(otherAttachedPos);

            Vec2D worldSpaceTargetDir = gobject->transform.local_to_scene_vector(attachDirection);
            Vec2D worldSpaceDir = myPointWorld-otherPointWorld;

            float dist = mean_length - worldSpaceDir.magnitude();
            worldSpaceDir.normalize();

            gobject->myScene->physics.AddImpulseAtLocation(gobject, myPointWorld, worldSpaceDir*dist*spring_constant*deltaTime);
            gobject->myScene->physics.AddImpulseAtLocation(otherObject, myPointWorld, -worldSpaceDir*dist*spring_constant*deltaTime);

            



            
        }
    }
}
