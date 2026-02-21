//#include "BaseEngine.h"
#include "Engine/GScene.h"
class GObject;
class CarScene : public GScene{

    public :
    void Tick_Objects(float  DeltaTime);
    void Begin();

    // private:
    float terrain[128];// = {15,35,15,15,15,45, 65, 10, 55, 25, 55, 12, 45, 12, 15, 25};
    float terrainPointSpacing = 100;
    int terrain_length = 128;
    float bottomYPos = -250;
    float initialOffset = -300;
    int terrainStart = 0;
    int terrainShownLength = 6;

    // private:
    ColliderShapeTemplate groundPolygons[15];
    ColliderShapeTemplate playerBody;

    ColliderShapeTemplate wheeltest;

    void setShapes();

    GObject* player;
    GObject* leftWheel;
    GObject* rightWheel;
    GObject* ground;

    GObject* head;
       
};