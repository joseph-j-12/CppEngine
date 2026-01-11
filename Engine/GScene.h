#ifndef HEADER_GScene
#define HEADER_GScene

#include "Vec2D.h"
#include <vector>
#include "GObject.h"
#include <memory>

/*
Scene contains all the objects. It runs the tick for all the objects and components. 
It connects rigidbodies and physics and also handles searching for children of an object
*/

class GScene{
    public :
    std::vector<std::unique_ptr<GObject>> sceneObjects;

};

#endif