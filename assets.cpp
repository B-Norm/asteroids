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
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

int WindowInfo::windowX = 1200;
int WindowInfo::windowY = 900;

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
        if(this->v.x * additionalV.x > 0) {
            additionalV.x *= 1.5;
        }
        if(this->v.y * additionalV.y > 0) {
            additionalV.y *= 1.5;
        }
        this->v += additionalV;
    } else if (key == sf::Keyboard::Key::Down) {
        //v -= .1;
    } else if(key == sf::Keyboard::Key::Right){
        object->rotate(2.0);
    } else if(key == sf::Keyboard::Key::Left){
        object->rotate(-2.0);
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

Projectile ProjectileFactory::createProjectile(sf::Vector2f origin, float angle) {
    return Projectile(origin, angle);
}


