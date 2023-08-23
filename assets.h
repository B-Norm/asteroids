#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

class WindowInfo{
    public:
        static int windowX;
        static int windowY;
};

class GameObject{
    public:
        std::string name;
        sf::Shape* object = nullptr;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f v;
        float angle;

        sf::Vector2f calcV(float mag);
        float calcMag(sf::Vector2f v);
        void updateLocation(); 
        void draw(sf::RenderWindow& window);
};

class Player : public GameObject {
    public:
        Player(int x, int y);
        void shipControls(sf::Keyboard::Key key);
        // void updateLocation();
};

class Projectile : public GameObject {
    public:
        Projectile(sf::Vector2f origin, float angle);
        bool collision();
        void setAge(int x);
        int getAge();

    private:
        int age;
};

class Asteroid : public GameObject {
    public:
        Asteroid();
};

class ProjectileFactory {
    public:
        Projectile createProjectile(sf::Vector2f origin, float angle);
};

#endif // ASSETS_H
