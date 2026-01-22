#include <iostream>
#include "BaseEngine.h"
#include "SFML/Graphics.hpp"
#include <array>

//to measure fps
#include <chrono>
#include <thread>

sf::Color hsvToRgb(float h, float s, float v);

Vec2D convertScreenToWorld(Vec2D screenPos, int screenwidth = 640, int screenheight= 480);
Vec2D convertWorldToScreen(Vec2D worldPos, int screenwidth = 640, int screenheight= 480);
void renderColliders(GScene* scene, sf::RenderWindow *window);

int screenW = 640;
int screenH = 480;
int main() {

    sf::RenderWindow window(sf::VideoMode(640,480),"app");

    GScene myScene;
    

    ColliderShapeTemplate shapetemplate1;
    shapetemplate1.numPoints = 4;
    shapetemplate1.points = (Vec2D*)malloc(4*sizeof(Vec2D));

    shapetemplate1.points[0].X = -10;
    shapetemplate1.points[0].Y = -10;

    shapetemplate1.points[1].X = -10;
    shapetemplate1.points[1].Y = 10;
    
    shapetemplate1.points[2].X = 10;
    shapetemplate1.points[2].Y = 10;

    shapetemplate1.points[3].X = 10;
    shapetemplate1.points[3].Y = -10;

    ColliderShapeTemplate staticObject;
    staticObject.numPoints = 4;
    staticObject.points = (Vec2D*)malloc(4*sizeof(Vec2D));

    staticObject.points[0].X = -105;
    staticObject.points[0].Y = -25;

    staticObject.points[1].X = -105;
    staticObject.points[1].Y = 25;
    
    staticObject.points[2].X = 105;
    staticObject.points[2].Y = 25;

    staticObject.points[3].X = 105;
    staticObject.points[3].Y = -25;
    std::chrono::duration<double, std::milli> duration;

    auto* newObj2 = myScene.AddNewObject<GObject>();

    newObj2->transform.position = Vec2D(0,-50);
    //newObj2->transform.rotation = 0.5f;
    newObj2->friction = 0.2f;

    auto* coll2 = newObj2->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &staticObject);

    for (int i = 0;i < 5; i++)
    {
        for (int j  = 0; j < 5; j++)
        {
            auto* newObj = myScene.AddNewObject<GObject>();
            if ((i+j)%2 == 0)
            auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Circle, 10);
            else
            auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &shapetemplate1);

            newObj->setPhysicsEnabled(true);
            newObj->transform.position = Vec2D(-10+i*20.5f,-10+j*20);
        }
    }

    auto* newObj = myScene.AddNewObject<GObject>();
    auto* coll = newObj->CreateComponent<GColliderComp>(GColliderComp::ColliderType::Polygon, &shapetemplate1);

    newObj->setPhysicsEnabled(true);
    newObj->transform.position = Vec2D(-600,60);
    newObj->friction = 0.9f;
    newObj->velocity = Vec2D(290,0);
    newObj->mass = 50;
    newObj->momentOfInertia = 15000;
    //newObj->angularVelocity = 5;

    myScene.Begin();
    while(window.isOpen())
    {
        auto start = std::chrono::steady_clock::now();
        //std::this_thread::sleep_for(std::chrono::milliseconds(7));

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
        }
        window.clear();
        //newObj->transform.calculateCosAndSine();
        //newObj->angularVelocity = 1.5;
        //newObj2->transform.position = posVec;

        myScene.Tick_Physics(duration.count() == 0 ? 0.00016 : 3*duration.count()/1000);

        renderColliders(&myScene, &window);
      
        window.display(); 
        auto end = std::chrono::steady_clock::now();
        duration = end - start;
        std::cout << "fps:" << 1000/duration.count() << std::endl;
    
        
        
    }

    return 0;
}

void renderColliders(GScene* scene, sf::RenderWindow *window)
{
    for (auto& obj : scene->sceneObjects)
    {
        for (auto& coll : obj->myComponents)
        {
            if (typeid(GColliderComp) == typeid(*coll))
            {
                auto* col = dynamic_cast<GColliderComp*>(coll.get());
                //std::cout << col->myColliderType << std::endl;
                if (col->myColliderType == GColliderComp::ColliderType::Polygon)
                {
                    sf::ConvexShape polygon;
                    polygon.setPointCount(col->myShape->numPoints);
                    
                    for (int i = 0; i < col->myShape->numPoints; i++)
                    {
                        Vec2D p1 = convertWorldToScreen(obj->transform.local_to_scene(col->myShape->points[i]), 640,480);
                        polygon.setPoint(i,sf::Vector2f(p1.X,p1.Y));    
                    }  
                    polygon.setFillColor(sf::Color::Transparent); 
                    polygon.setOutlineColor(sf::Color::Red); 
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
                    Vec2D pos = convertWorldToScreen(col->gobject->transform.position);
                    circle.setPosition(pos.X-col->circleRadius, pos.Y-col->circleRadius);
                    circle.setOutlineColor(sf::Color::Red); // Set a red outline color
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineThickness(1.f);
                    window->draw(circle);

                    Vec2D linePos = col->gobject->transform.local_to_scene(Vec2D(col->circleRadius,0));
                    Vec2D linescenepos = convertWorldToScreen(linePos);
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(pos.X, pos.Y)), // Start point (x1, y1)
                        sf::Vertex(sf::Vector2f(linescenepos.X, linescenepos.Y))  // End point (x2, y2)
                    };
                    window->draw(line, 2, sf::Lines);
                }
            }
        }
        //std::cout << obj->velocity.Y << std::endl;
    }
}

Vec2D convertScreenToWorld(Vec2D screenPos, int screenwidth, int screenheight)
{
    return Vec2D(screenPos.X-screenwidth/2,-(screenPos.Y-screenheight/2));
}

Vec2D convertWorldToScreen(Vec2D worldPos, int screenwidth, int screenheight)
{
    return Vec2D(worldPos.X+screenwidth/2, (-worldPos.Y)+screenheight/2);
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