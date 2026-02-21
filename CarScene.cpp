//#include "CarScene.h"
#include "BaseEngine.h"
//#include ""
#include <cstdlib>
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
    ground->bounce = 0.2f;

    int lastHeight = 15;
    for (int i = 0; i < terrain_length; i++)
    {
        terrain[i] = (lastHeight + (10-std::rand()%20))*(std::sin(lastHeight*i/2)+1);
        lastHeight = terrain[i];
    }
    

    playerBody.numPoints = 4;
    playerBody.points = new Vec2D[4];
    //playerBody.points = {new Vec2D(-105,-25), new Vec2D(-105,25), new Vec2D(105,25), new Vec2D(105,-25)};
    playerBody.points[0].X = -45;
    playerBody.points[0].Y = -8;

    playerBody.points[1].X = -35;
    playerBody.points[1].Y = 8;
    
    playerBody.points[2].X = 35;
    playerBody.points[2].Y = 8;

    playerBody.points[3].X = 45;
    playerBody.points[3].Y = -8;

    wheeltest.numPoints = 7;
    wheeltest.points = new Vec2D[7];
    for (int i = 0; i < 7; i++)
    {
        wheeltest.points[i] = Vec2D(30*std::cos(i*6.28f/7),30*std::sin(i*6.28f/7));
    }

    rightWheel = AddNewObject<GObject>();
    auto* coll4 = rightWheel->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 8);
    rightWheel->setPhysicsEnabled(true);
    rightWheel->transform.position = Vec2D(55, 150);
    rightWheel->mass = 25;
    rightWheel->transform.rotation = 1.5f;
    rightWheel->momentOfInertia = 5000;
    rightWheel->bounce = 0.1f;
    rightWheel->friction = 0.8f;
    rightWheel->angularVelocity = -5.f;

    leftWheel = AddNewObject<GObject>();
    auto* coll = leftWheel->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 8);
    leftWheel->setPhysicsEnabled(true);
    leftWheel->transform.position = Vec2D(35, 150);
    leftWheel->mass = 25;
    leftWheel->transform.rotation = 1.5f;
    leftWheel->momentOfInertia = 5000;
    leftWheel->bounce = 0.1f;
    leftWheel->friction = 0.8f;

    player =  AddNewObject<GObject>();
    coll = player->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &playerBody, Vec2D(0,0));
    // coll = player->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 8, Vec2D(35,0));
    // coll = player->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 8, Vec2D(-35,0));
    player->setPhysicsEnabled(true);
    player->transform.position = Vec2D(45, 200);
    player->my_id = 1;
    auto* spring = player->CreateComponent<GSpringComponent>(leftWheel);
    spring->spring_constant = 3000.f;
    spring->constrainedAlongAxis = true;
    spring->myAttachedPos = Vec2D(-25,0);
    spring->attachDirection = Vec2D(0,-1);
    spring->mean_length = 30.f;
    spring->minLength = 14;
    spring->maxLength = 155;

    auto* spring2 = player->CreateComponent<GSpringComponent>(rightWheel);
    spring2->spring_constant = 3000.f;
    spring2->constrainedAlongAxis = true;
    spring2->myAttachedPos = Vec2D(25,0);
    //spring->otherAttachedPos = Vec2D(0,25);
    spring2->attachDirection = Vec2D(0,-1);
    spring2->mean_length = 30.f;
    spring2->minLength = 14;
    spring2->maxLength = 155;

    //newObj->velocity = Vec2D(0,-100);
    player->friction = 0.3f;
    player->mass = 1000;
    player->momentOfInertia = 2000000;
    player->bounce = 0.2f;

    head = AddNewObject<GObject>();
    coll = head->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 5);
    head->setPhysicsEnabled(true);

    head->mass = 50;
    head->momentOfInertia = 50000;
    spring = player->CreateComponent<GSpringComponent>(head);
    spring->mean_length = 30.f;
    spring->spring_constant = 1000.f;
    spring->constrainedAlongAxis = true;
    spring->attachDirection = Vec2D(0,1);
    head->transform.position = Vec2D(45, 225);
    

    GScene::Begin();
}

void CarScene::setShapes()
{
    if (terrainStart < 0) terrainStart = 0;
    if (terrainStart > terrain_length - terrainShownLength)
    terrainStart = terrain_length - terrainShownLength;

    int terrainShowend = terrainStart + terrainShownLength;
    
    // std::cout << terrainShowend << std::endl;
    for (int i = terrainStart; i < terrainShowend; i++)
    {
        
        int j = i - terrainStart;
        groundPolygons[j].points[3] = Vec2D(initialOffset+i*terrainPointSpacing,bottomYPos);
        groundPolygons[j].points[2] = Vec2D(initialOffset+(i+1)*terrainPointSpacing,bottomYPos);
        groundPolygons[j].points[1] = Vec2D(initialOffset+(i+1)*terrainPointSpacing,terrain[i+1]);
        groundPolygons[j].points[0] = Vec2D(initialOffset+i*terrainPointSpacing,terrain[i]);
    }
}
