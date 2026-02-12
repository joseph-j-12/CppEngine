//#include "CarScene.h"
#include "BaseEngine.h"
//#include ""
#include <iostream>

void CarScene::Tick_Objects(float DeltaTime)
{
    GScene::Tick_Objects(DeltaTime);
    setShapes();
    // leftWheel->angularVelocity = 5;
    // rightWheel->angularVelocity = 5;
    //std::cout << sceneObjects.size() << std::endl;
}

void CarScene::Begin()
{   
    ground = AddNewObject<GObject>();
    for (int i = 0; i < terrainShownLength; i++)
    {
        groundPolygons[i].points = new Vec2D[4];
        groundPolygons[i].numPoints = 4;
        ground->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &groundPolygons[i]);
    }
    ground->friction = 0.9f;
    ground->bounce = 0.8f;

    playerBody.numPoints = 4;
    playerBody.points = new Vec2D[4];
    //playerBody.points = {new Vec2D(-105,-25), new Vec2D(-105,25), new Vec2D(105,25), new Vec2D(105,-25)};
    playerBody.points[0].X = -105;
    playerBody.points[0].Y = -25;

    playerBody.points[1].X = -105;
    playerBody.points[1].Y = 25;
    
    playerBody.points[2].X = 105;
    playerBody.points[2].Y = 25;

    playerBody.points[3].X = 105;
    playerBody.points[3].Y = -25;

    wheeltest.numPoints = 7;
    wheeltest.points = new Vec2D[7];
    for (int i = 0; i < 7; i++)
    {
        wheeltest.points[i] = Vec2D(30*std::cos(i*6.28f/7),30*std::sin(i*6.28f/7));
    }

    rightWheel = AddNewObject<GObject>();
    auto* coll4 = rightWheel->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 25);
    rightWheel->setPhysicsEnabled(true);
    rightWheel->transform.position = Vec2D(100, 100);
    rightWheel->mass = 150;
    rightWheel->transform.rotation = 1.5f;
    rightWheel->momentOfInertia = 30000;
    rightWheel->bounce = 0.1f;
    rightWheel->friction = 0.8f;
    rightWheel->angularVelocity = -5.f;

    leftWheel = AddNewObject<GObject>();
    auto* coll = leftWheel->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 25);
    leftWheel->setPhysicsEnabled(true);
    leftWheel->transform.position = Vec2D(-10, 100);
    leftWheel->mass = 150;
    leftWheel->transform.rotation = 1.5f;
    leftWheel->momentOfInertia = 30000;
    leftWheel->bounce = 0.1f;
    leftWheel->friction = 0.8f;

    player =  AddNewObject<GObject>();
    coll = player->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &playerBody, Vec2D(0,0));
    player->setPhysicsEnabled(true);
    player->transform.position = Vec2D(45, 200);
    player->my_id = 1;
    auto* spring = player->CreateComponent<GSpringComponent>(leftWheel);
    spring->spring_constant = 1000.f;
    spring->constrainedAlongAxis = true;
    spring->myAttachedPos = Vec2D(-55,0);
    spring->attachDirection = Vec2D(0,-1);
    spring->mean_length = 90.f;
    spring->minLength = 60;
    spring->maxLength = 155;

    auto* spring2 = player->CreateComponent<GSpringComponent>(rightWheel);
    spring2->spring_constant = 1000.f;
    spring2->constrainedAlongAxis = true;
    spring2->myAttachedPos = Vec2D(55,0);
    //spring->otherAttachedPos = Vec2D(0,25);
    spring2->attachDirection = Vec2D(0,-1);
    spring2->mean_length = 90.f;
    spring2->minLength = 60;
    spring2->maxLength = 155;

    //newObj->velocity = Vec2D(0,-100);
    player->friction = 0.3f;
    player->mass = 2800;
    player->momentOfInertia = 1000000;
    player->bounce = 0.5f;

    GScene::Begin();
}

void CarScene::setShapes()
{
    int terrainShowend = terrainStart +  terrainShownLength;
    if (terrainShowend > terrain_length) terrainShowend = terrain_length;
    if (terrainStart < 0) terrainStart = 0;
    for (int i = terrainStart; i < terrainShowend; i++)
    {
        int j = i - terrainStart;
        groundPolygons[j].points[3] = Vec2D(initialOffset+i*terrainPointSpacing,bottomYPos);
        groundPolygons[j].points[2] = Vec2D(initialOffset+(i+1)*terrainPointSpacing,bottomYPos);
        groundPolygons[j].points[1] = Vec2D(initialOffset+(i+1)*terrainPointSpacing,terrain[i+1]);
        groundPolygons[j].points[0] = Vec2D(initialOffset+i*terrainPointSpacing,terrain[i]);
    }
}
