#include <iostream>
#include "BaseEngine.h"
#include "SFML/Graphics.hpp"
#include <thread>


sf::Color hsvToRgb(float h, float s, float v);

Vec2D convertScreenToWorld(Vec2D screenPos, int screenwidth, int screenheight);
Vec2D convertWorldToScreen(Vec2D worldPos, int screenwidth, int screenheight);

int main() {

    sf::RenderWindow window(sf::VideoMode(640,480),"app");
    Vec2D v = Vec2D(2,3.5f) + -Vec2D(4.5f,1);
    v.normalize();
    v = Vec2D(2,3);

    GTransform t;
    GTransform t2;
    GTransform moon;
    t2.position = Vec2D(250,250);
    t.position = Vec2D(100,0);
    moon.position = Vec2D(25,0);
    t2.rotation = 3.1415f;
    t.parent = &t2;
    moon.parent = &t;


    Vec2D v2 = t.local_to_scene(Vec2D());

    std::cout << v2.X << " - " << v2.Y << std::endl;
    sf::CircleShape shape1(25);
    sf::CircleShape shape2(9);
    sf::CircleShape shape3(4);
    while(window.isOpen())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        Vec2D posVec = Vec2D(pos.x, pos.y);
        posVec = convertScreenToWorld(posVec, 640,480);        
        posVec = convertWorldToScreen(posVec, 640,480);
        std::cout << posVec.X << " - " << posVec.Y << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear();
        t2.position = posVec;
        window.draw(shape1);
        shape1.setPosition(t2.position.X-25, t2.position.Y-25);

        window.draw(shape2);
        Vec2D X = t.myScenePosition();
        shape2.setPosition(X.X-9, X.Y-9);
        
        window.draw(shape3);
        X = moon.myScenePosition();
        shape3.setPosition(X.X-4, X.Y-4);
        shape2.setFillColor(hsvToRgb(55,1,1));
        window.display();
        t2.rotation += 0.1;
        t.rotation += 0.05;

        t.position.X = 100+25*std::sin(t2.rotation*5);
    }

    return 0;
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