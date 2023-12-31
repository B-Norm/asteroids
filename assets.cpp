#include "assets.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

int WindowInfo::windowX = 1200;
int WindowInfo::windowY = 900;

int Debris::count = 18;

sf::Vector2f GameObject::calcV(float mag) {
    float x, y;
    float PI = 3.14159265;

    float angle = (object->getRotation() - 90) * PI / 180.0 ;
    // get magnitude of current Vector2

    x = mag * std::cos(angle);
    y = mag * std::sin(angle);

    sf::Vector2f newV(x, y);
    return newV;
}
float GameObject::calcMag(sf::Vector2f v){
    return std::sqrt(v.x * v.x + v.y + v.y);
}
void GameObject::updateLocation() {
    object->move(v);
    // if object goes off screen make it appear on the oposite side
    if(object->getPosition().x < 0) {
        object->setPosition(WindowInfo::windowX, object->getPosition().y);
    } else if(object->getPosition().x > WindowInfo::windowX) {
        object->setPosition(0.0, object->getPosition().y);
    } else if(object->getPosition().y < 0) {
        object->setPosition(object->getPosition().x, WindowInfo::windowY);
    } else if(object->getPosition().y > WindowInfo::windowY) {
        object->setPosition(object->getPosition().x, 0.0);
    }
}
void GameObject::draw(sf::RenderWindow& window) {
    window.draw(*object);
}

Player::Player(int x, int y) {
    this->name = "Player";

    // create triangle for player------------------------------
    sf::ConvexShape* triangle = new sf::ConvexShape();
    triangle->setPointCount(3);
    triangle->setPoint(0,sf::Vector2f(0, 40));
    triangle->setPoint(1,sf::Vector2f(20, 40));
    triangle->setPoint(2,sf::Vector2f(10, 0));
    triangle->setFillColor(sf::Color::Black);
    triangle->setOutlineColor(sf::Color::White);
    triangle->setOutlineThickness(1);

    object = triangle;
    object->setPosition(x, y);
    object->setOrigin(10,20);
    this->v.x = 0.0;
    this->v.y = 0.0;
}
void Player::shipControls(sf::Keyboard::Key key) {
    if(key == sf::Keyboard::Key::Up){
        sf::Vector2f additionalV = calcV(0.1);
        if(this->v.x * additionalV.x < 0) {
            additionalV.x *= 3;
        }
        if(this->v.y * additionalV.y < 0) {
            additionalV.y *= 3;
        }
        this->v += additionalV;
    } else if (key == sf::Keyboard::Key::Down) {
        //v -= .1;
    } else if(key == sf::Keyboard::Key::Right){
        object->rotate(3.0);
    } else if(key == sf::Keyboard::Key::Left){
        object->rotate(-3.0);
    } 
}

Projectile::Projectile(sf::Vector2f origin, float angle) {
    this->name = "Projectile";
    this->age = 0;
    sf::RectangleShape* bullet = new sf::RectangleShape();
    bullet->setSize(sf::Vector2f(3, 3));
    bullet->setFillColor(sf::Color::White);

    object = bullet;
    object->setPosition(origin);
    object->setOrigin(1.5, 1.5);
    object->setRotation(angle);
    this->v += calcV(10.0);

}
void Projectile::setAge(int x) {
    this->age = x;
}
int Projectile::getAge() {
    return this->age;
}

Asteroid::Asteroid(bool adult, sf::Vector2f spawnVariable, int seed) {
    unsigned int xRand, yRand;
    int angleRand;

    this->name = "Asteroid";
    sf::ConvexShape* asteroid = new sf::ConvexShape();
    asteroid->setPointCount(6);
    asteroid->setPoint(0, sf::Vector2f(0, 45));
    asteroid->setPoint(1, sf::Vector2f(3, 60));
    asteroid->setPoint(2, sf::Vector2f(55, 50));
    asteroid->setPoint(3, sf::Vector2f(65, 25));
    asteroid->setPoint(4, sf::Vector2f(45, 5));
    asteroid->setPoint(5, sf::Vector2f(15, 0));
    asteroid->setFillColor(sf::Color::Black);
    asteroid->setOutlineColor(sf::Color::White);
    asteroid->setOutlineThickness(1);

    if(adult) {
        this->adult = adult;        
        // logic for random asteroid placement
        srand(seed);
        unsigned int minRangeX = spawnVariable.x - 200;
        unsigned int maxRangeX = spawnVariable.x + 200;
        unsigned int minRangeY = spawnVariable.y - 200;
        unsigned int maxRangeY = spawnVariable.y + 200;
        unsigned int largerMinX = 0;
        unsigned int largerMaxX = WindowInfo::windowX;
        unsigned int largerMinY = 0;
        unsigned int largerMaxY = WindowInfo::windowY;

        // get rand # inside range
        xRand = largerMinX + rand() % (largerMaxX - largerMinX + 1);
        yRand = largerMinY + rand() % (largerMaxY - largerMinY + 1);

        // make sure that it is away from the asteroid
        if(xRand >= minRangeX && xRand <= maxRangeX) {
            if(rand() % 2 == 0) {
                xRand = largerMaxX + rand() % (largerMaxX - maxRangeX + 1);
            } else {
                xRand = largerMinX + rand() % (minRangeX - largerMinX + 1);
            }
        }
        if(yRand >= minRangeY && yRand <= maxRangeY) {
            if(rand() % 2 == 0) {
                yRand = largerMaxY + rand() % (largerMaxY - maxRangeY + 1);
            } else {
                yRand = largerMinY + rand() % (minRangeY - largerMinY + 1);
            }
        }
        // logic for random angle
        angleRand = rand() % (360 + 1);

        asteroid->setScale(3.0, 3.0);
    } else {
        this->adult = adult;
        xRand = spawnVariable.x;
        yRand = spawnVariable.y;
        angleRand = rand() % (360 + 1);
    }

    this->object = asteroid;

    // randomly genreate locations and rotations for asteroids
    this->object->setPosition(xRand, yRand);
    this->object->setOrigin(33, 33);
    // random angles for asteroid orientation
    this->object->setRotation(angleRand);
    this->v = calcV(1);

}
bool Asteroid::getAdult() {
    return this->adult;
}

Debris::Debris(sf::Vector2f spawn, int range, int index, int seed) {
    srand(seed);
    unsigned int randAngle = rand() % (range + 1);
    randAngle += (range * index);


    sf::RectangleShape* debris = new sf::RectangleShape();
    debris->setSize(sf::Vector2f(1, 1));
    debris->setFillColor(sf::Color::White);
    
    object = debris;
    object->setPosition(spawn);
    object->setRotation(randAngle);
    this->v = calcV(.5);

}
void Debris::setLife(int life) {
    this->life = life;
}
int Debris::getLife(){
    return this->life;
}

Projectile ProjectileFactory::createProjectile(sf::Vector2f origin, float angle) {
    return Projectile(origin, angle);
}
Asteroid AsteroidFactory::createAsteroid(bool adult, sf::Vector2f spawnVariable, int seed) {
    return Asteroid(adult, spawnVariable, seed);
}
Debris DebrisFactory::createDebris(sf::Vector2f spawn, int range, int index, int seed) {
    return Debris(spawn, range, index, seed);
}


