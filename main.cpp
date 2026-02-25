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
void renderColliders(GScene* scene, sf::RenderWindow *window, GCamera* ca);

int screenW = 320;
int screenH = 240;

float scrollx = 0;
float scrolly = 0;
int main() {

    sf::RenderWindow window(sf::VideoMode(320,240),"app");

    CarScene myScene;

    std::chrono::duration<double, std::milli> duration;


    // for (int i = 0;i < 3; i++)
    // {
    //     for (int j  = 0; j < 50; j++)
    //     {
    //         auto* newObj = myScene.AddNewObject<GObject>();
    //        // if ((i+j)%2 == 0)
    //         auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 5);
    //         //else
    //         //auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &shapetemplate1);

    //         newObj->setPhysicsEnabled(true);
    //         newObj->transform.position = Vec2D(-200+i*20.5f,300+j*20);
    //         newObj->mass = 1;
    //         newObj->momentOfInertia = 100;
    //         newObj->bounce = 0.6f;
    //         newObj->friction = 0.3f;
    //         //newObj->angularVelocity = (-1+ ((i+j)%2)*2)*200;
    //     }
    // }

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
    GCamera cam(&myScene);
    myScene.cam.screenSize = Vec2D(320,240);
    myScene.Begin();
    myScene.cam.trackingTarget = myScene.player;
    myScene.cam.trackingSpeed = Vec2D(3.f, 1.f);
    myScene.cam.zoom = 0.8f;
    myScene.cam.maxTrackDistance = 100;
    while(window.isOpen())
    {
        scrollx = myScene.cam.position.X;// += (myScene.player->transform.position.X - scrollx)*0.08;
        scrolly = myScene.cam.position.Y; //+= (myScene.player->transform.position.Y - scrolly)*0.01;

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
        renderColliders(&myScene, &window, &myScene.cam);
      
        window.display(); 
        auto end = std::chrono::steady_clock::now();
        duration = end - start;
    //   std::cout << "fps:" << 1000/duration.count() << std::endl;
    
        
        
    }

    return 0;
}

void renderColliders(GScene* scene, sf::RenderWindow *window, GCamera* cam)
{
    for (auto& obj : scene->sceneObjects)
    {
        if (!obj->enabled) continue;
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
                        Vec2D p1 = cam->convertWorldToScreen(obj->transform.local_to_scene(col->myShape->points[i] + col->myCenter));
                        polygon.setPoint(i,sf::Vector2f(p1.X,p1.Y));    
                    }  
                    polygon.setFillColor(sf::Color::Transparent); 
                    
                    polygon.setOutlineColor(obj->getPhysicsEnabled() ?  mycolor : sf::Color::Blue); 
                    polygon.setOutlineThickness(2.f);
                    window->draw(polygon); 

                    sf::RectangleShape bound;
                    col->CalculateBoundingBox();
                    Vec2D bTop = cam->convertWorldToScreen(Vec2D(col->boundingBoxMin.X, col->boundingBoxMax.Y));
                    Vec2D bBottom = cam->convertWorldToScreen(Vec2D(col->boundingBoxMax.X, col->boundingBoxMin.Y));
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
                    sf::CircleShape circle(col->circleRadius*cam->zoom);
                    circle.setFillColor(sf::Color::Green); // Set the fill color to green
                    Vec2D pos = cam->convertWorldToScreen(col->gobject->transform.local_to_scene(col->myCenter));
                    circle.setPosition(pos.X-col->circleRadius*cam->zoom, pos.Y-col->circleRadius*cam->zoom);
                    circle.setOutlineColor(obj->getPhysicsEnabled() ?  sf::Color::Red : sf::Color::Blue); // Set a red outline color
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineThickness(1.f);
                    window->draw(circle);

                    Vec2D linePos = col->gobject->transform.local_to_scene(col->myCenter+Vec2D(col->circleRadius,0));
                    Vec2D linescenepos = cam->convertWorldToScreen(linePos);
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
                if (!spring->gobject->enabled || !spring->otherObject->enabled) continue;
                Vec2D linePos = spring->gobject->transform.local_to_scene(spring->myAttachedPos);
                Vec2D linePos2 = spring->otherObject->transform.local_to_scene(spring->otherAttachedPos);
                Vec2D linescenepos1 = cam->convertWorldToScreen(linePos);
                Vec2D linescenepos2 = cam->convertWorldToScreen(linePos2);
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