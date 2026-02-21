#include <iostream>
#include "BaseEngine.h"
#include "SFML/Graphics.hpp"
#include <array>

//to measure fps
#include <chrono>
#include <thread>

sf::Color hsvToRgb(float h, float s, float v);

Vec2D convertScreenToWorld(Vec2D screenPos, int screenwidth = 320, int screenheight= 240);
Vec2D convertWorldToScreen(Vec2D worldPos, int screenwidth = 320, int screenheight= 240);
void renderColliders(GScene* scene, sf::RenderWindow *window);

int screenW = 320;
int screenH = 240;

float scrollx = 0;
float scrolly = 0;
int main() {

    sf::RenderWindow window(sf::VideoMode(320,240),"app");

    CarScene myScene;
    


    // ColliderShapeTemplate shapetemplate1;
    // shapetemplate1.numPoints = 4;
    // shapetemplate1.points = new Vec2D[4];//(Vec2D*)malloc(4*sizeof(Vec2D));

    // shapetemplate1.points[0].X = -10;
    // shapetemplate1.points[0].Y = -10;

    // shapetemplate1.points[1].X = -10;
    // shapetemplate1.points[1].Y = 10;
    
    // shapetemplate1.points[2].X = 10;
    // shapetemplate1.points[2].Y = 10;

    // shapetemplate1.points[3].X = 10;
    // shapetemplate1.points[3].Y = -10;


    // ColliderShapeTemplate shapetemplate3;
    // shapetemplate3.numPoints = 7;
    // shapetemplate3.points = new Vec2D[7];
    // for (int i = 0; i < 7; i++)
    // {
    //     shapetemplate3.points[i] = Vec2D(30*std::cos(i*6.28f/7),30*std::sin(i*6.28f/7));
    // }


    // ColliderShapeTemplate staticObject;
    // staticObject.numPoints = 4;
    // staticObject.points = new Vec2D[4];

    // staticObject.points[3].X = -105;
    // staticObject.points[3].Y = -25;

    // staticObject.points[2].X = -105;
    // staticObject.points[2].Y = 25;
    
    // staticObject.points[1].X = 105;
    // staticObject.points[1].Y = 25;

    // staticObject.points[0].X = 105;
    // staticObject.points[0].Y = -25;

    // ColliderShapeTemplate staticObject2;
    // staticObject2.numPoints = 4;
    // staticObject2.points = new Vec2D[4];

    // staticObject2.points[0].X = -25;
    // staticObject2.points[0].Y = -25;

    // staticObject2.points[1].X = -25;
    // staticObject2.points[1].Y = 25;
    
    // staticObject2.points[2].X = 25;
    // staticObject2.points[2].Y = 25;

    // staticObject2.points[3].X = 25;
    // staticObject2.points[3].Y = -25;

    std::chrono::duration<double, std::milli> duration;

    // auto* newObj2 = myScene.AddNewObject<GObject>();

    // newObj2->transform.position = Vec2D(-100,-170);
    // newObj2->transform.rotation =- 0.3f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.3f;

    // auto* coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);

    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(-255,-100);
    // newObj2->transform.rotation = -0.6f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.3f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);


    // // newObj2 = myScene.AddNewObject<GObject>();
    // // newObj2->transform.position = Vec2D(100,100);
    // // newObj2->transform.rotation = 0.6f;
    // // newObj2->friction = 1.8f;
    // // newObj2->bounce = 0.8f;
    // // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);



    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(55,-210);
    // newObj2->transform.rotation = -0.2f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.8f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);

    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(205,-220);
    // newObj2->transform.rotation = 0.0f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.8f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);

    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(305,-220);
    // newObj2->transform.rotation = 0.4f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.8f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);

    // // newObj2 = myScene.AddNewObject<GObject>();
    // // newObj2->transform.position = Vec2D(-30,-190);
    // // newObj2->transform.rotation = 0.4f;
    // // newObj2->friction = 1.f;
    // // newObj2->bounce = 0.8f;
    // // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject2);

    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(405,-120);
    // newObj2->transform.rotation = 1.4f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.8f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);


    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(-325,50);
    // newObj2->transform.rotation = 1.4f;
    // newObj2->friction = 0.9f;
    // newObj2->bounce = 0.8f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);
    // newObj2 = myScene.AddNewObject<GObject>();
    // newObj2->transform.position = Vec2D(-0,30);
    // MATLAB Function1newObj2->friction = 0.2f;
    // newObj2->bounce = 0.3f;
    // newObj2->transform.rotation = 0.78f;
    // coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 30);

    for (int i = 0;i < 3; i++)
    {
        for (int j  = 0; j < 50; j++)
        {
            auto* newObj = myScene.AddNewObject<GObject>();
           // if ((i+j)%2 == 0)
            auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 5);
            //else
            //auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &shapetemplate1);

            newObj->setPhysicsEnabled(true);
            newObj->transform.position = Vec2D(-200+i*20.5f,300+j*20);
            newObj->mass = 1;
            newObj->momentOfInertia = 100;
            newObj->bounce = 0.6f;
            newObj->friction = 0.3f;
            //newObj->angularVelocity = (-1+ ((i+j)%2)*2)*200;
        }
    }

    // for (int j  = 0; j < 15; j++)
    // {
    //     auto* newObj = myScene.AddNewObject<GObject>();
    //    // if ((i+j)%2 == 0)
    //     auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 10);
    //     //else
    //     //auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &shapetemplate1);
    //     newObj->setPhysicsEnabled(true);
    //     newObj->transform.position = Vec2D(-350+j*22, -10);
    //     newObj->mass = 1000;
    //     newObj->momentOfInertia = 100000;
    //     newObj->bounce = 0.1f;
    //     newObj->friction = 1.5f;
    //     newObj->angularVelocity = -500.f;
    //     //newObj->angularVelocity = (-1+ ((i+j)%2)*2)*2;
    // }

  
    //newObj3->angularVelocity = -15.f;
    

    //coll = newObj3->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 25, Vec2D(100,0));
    //coll = newObj3->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 25, Vec2D(-100,0));
    GCamera cam;
    myScene.Begin();
    cam.trackingTarget = myScene.player;
    while(window.isOpen())
    {
        scrollx = cam.position.X;// += (myScene.player->transform.position.X - scrollx)*0.08;
        scrolly = cam.position.Y; //+= (myScene.player->transform.position.Y - scrolly)*0.01;

        myScene.terrainStart = round((scrollx-25)/myScene.terrainPointSpacing);
        auto start = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        cam.Tick(0.0016f);
        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        Vec2D posVec = Vec2D(pos.x, pos.y);
        posVec = convertScreenToWorld(posVec, 640,480);        
        //posVec = convertWorldToScreen(posVec, 640,480);
        //std::cout << posVec.X << " - " << posVec.Y << std::endl;
        //newObj2->transform.rotation = posVec.X/500;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                // Check specific keys
                if (event.key.code == sf::Keyboard::Left)
                {
                    myScene.leftWheel->angularVelocity = 25.f;
                    myScene.rightWheel->angularVelocity = 25.f;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    myScene.leftWheel->angularVelocity = -25.f;
                    myScene.rightWheel->angularVelocity = -25.f;
                }
            }
        }
        window.clear();
        //newObj->transform.calculateCosAndSine();
        //newObj->angularVelocity = 1.5;
        //newObj2->transform.position = posVec;

        myScene.Tick_Physics(duration.count() == 0 ? 0.00016 : 3.f*duration.count()/1000);
        myScene.Tick_Objects(duration.count() == 0 ? 0.00016 : 3.f*duration.count()/1000);
        renderColliders(&myScene, &window);
      
        window.display(); 
        auto end = std::chrono::steady_clock::now();
        duration = end - start;
    //   std::cout << "fps:" << 1000/duration.count() << std::endl;
    
        
        
    }

    return 0;
}

void renderColliders(GScene* scene, sf::RenderWindow *window)
{
    for (auto& obj : scene->sceneObjects)
    {
        sf::Color mycolor =  obj->my_id == 1 ? sf::Color::Yellow : sf::Color::Red;
        for (auto& coll : obj->myComponents)
        {
            if (typeid(GColliderComp) == typeid(*coll))
            {
                auto* col = dynamic_cast<GColliderComp*>(coll.get());
                
                if (col->myColliderType == GColliderComp::ColliderType::Polygon)
                {
                    sf::ConvexShape polygon;
                    polygon.setPointCount(col->myShape->numPoints);
                    
                    for (int i = 0; i < col->myShape->numPoints; i++)
                    {
                        Vec2D p1 = convertWorldToScreen(obj->transform.local_to_scene(col->myShape->points[i] + col->myCenter));
                        polygon.setPoint(i,sf::Vector2f(p1.X,p1.Y));    
                    }  
                    polygon.setFillColor(sf::Color::Transparent); 
                    
                    polygon.setOutlineColor(obj->getPhysicsEnabled() ?  mycolor : sf::Color::Blue); 
                    polygon.setOutlineThickness(2.f);
                    window->draw(polygon); 

                    sf::RectangleShape bound;
                    col->CalculateBoundingBox();
                    Vec2D bTop = convertWorldToScreen(Vec2D(col->boundingBoxMin.X, col->boundingBoxMax.Y));
                    Vec2D bBottom = convertWorldToScreen(Vec2D(col->boundingBoxMax.X, col->boundingBoxMin.Y));
                    //std::cout << bBottom.Y << "-" << bTop.Y << std::endl;
                    bound.setPosition(bTop.X, bTop.Y);
                    bound.setSize(sf::Vector2(bBottom.X-bTop.X, bBottom.Y-bTop.Y));
                    bound.setFillColor(sf::Color::Transparent);
                    bound.setOutlineColor(sf::Color::Green);
                    bound.setOutlineThickness(1.f);
                    //window->draw(bound);

                }
                else if (col->myColliderType == GColliderComp::ColliderType::Circle)
                {
                    sf::CircleShape circle(col->circleRadius);
                    circle.setFillColor(sf::Color::Green); // Set the fill color to green
                    Vec2D pos = convertWorldToScreen(col->gobject->transform.local_to_scene(col->myCenter));
                    circle.setPosition(pos.X-col->circleRadius, pos.Y-col->circleRadius);
                    circle.setOutlineColor(obj->getPhysicsEnabled() ?  sf::Color::Red : sf::Color::Blue); // Set a red outline color
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineThickness(1.f);
                    window->draw(circle);

                    Vec2D linePos = col->gobject->transform.local_to_scene(col->myCenter+Vec2D(col->circleRadius,0));
                    Vec2D linescenepos = convertWorldToScreen(linePos);
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(pos.X, pos.Y)), // Start point (x1, y1)
                        sf::Vertex(sf::Vector2f(linescenepos.X, linescenepos.Y))  // End point (x2, y2)
                    };
                    window->draw(line, 2, sf::Lines);
                }
            }

            else if (typeid(GSpringComponent) == typeid(*coll))
            {
                auto* spring = dynamic_cast<GSpringComponent*>(coll.get());
                Vec2D linePos = spring->gobject->transform.local_to_scene(spring->myAttachedPos);
                Vec2D linePos2 = spring->otherObject->transform.local_to_scene(spring->otherAttachedPos);
                Vec2D linescenepos1 = convertWorldToScreen(linePos);
                Vec2D linescenepos2 = convertWorldToScreen(linePos2);
                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(linescenepos2.X, linescenepos2.Y)), // Start point (x1, y1)
                    sf::Vertex(sf::Vector2f(linescenepos1.X, linescenepos1.Y))  // End point (x2, y2)
                };
                window->draw(line, 2, sf::Lines);
            }
        }
        
    }
}

Vec2D convertScreenToWorld(Vec2D screenPos, int screenwidth, int screenheight)
{
    return Vec2D(screenPos.X-screenwidth/2,-(screenPos.Y-screenheight/2));
}

Vec2D convertWorldToScreen(Vec2D worldPos, int screenwidth, int screenheight)
{
    return Vec2D((worldPos.X-scrollx)+screenwidth/2, (-worldPos.Y+scrolly)+screenheight/2);
}


sf::Color hsvToRgb(float h, float s, float v) {
    // h: 0–360, s: 0–1, v: 0–1
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;

    if (h < 60)      { r = c; g = x; b = 0; }
    else if (h < 120){ r = x; g = c; b = 0; }
    else if (h < 180){ r = 0; g = c; b = x; }
    else if (h < 240){ r = 0; g = x; b = c; }
    else if (h < 300){ r = x; g = 0; b = c; }
    else             { r = c; g = 0; b = x; }

    sf::Uint8 R = static_cast<sf::Uint8>((r + m) * 255);
    sf::Uint8 G = static_cast<sf::Uint8>((g + m) * 255);
    sf::Uint8 B = static_cast<sf::Uint8>((b + m) * 255);

    return sf::Color(R, G, B);
}