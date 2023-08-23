#include "assets.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <clocale>
#include <iostream>
#include <SFML/Window.hpp>
#include <cmath>
#include <string>

// TODO: load in asteroids and collision detection

int main () {
    sf::RenderWindow window(sf::VideoMode(WindowInfo::windowX, WindowInfo::windowY), "Asteroidz");
    window.setPosition(sf::Vector2i(700,20));
    window.setFramerateLimit(60);

    // vector for bullets
    std::vector<Projectile> projectiles;
    ProjectileFactory projectileFactory;

    // vector for asteroids
    std::vector<Asteroid> asteroids;
    AsteroidFactory asteroidFactory;
    asteroids.push_back(asteroidFactory.createAsteroid(true));
    // load in player
    Player player(WindowInfo::windowX/2, WindowInfo::windowY/2);

    // debug info for player-----------------------------------
    sf::Font font;
    font.loadFromFile("runescape_uf.ttf");
    sf::Text info("stuff", font, 24);
    info.setFillColor(sf::Color::White);
    info.setPosition(10.f,10.f);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
                std::cout << "Space Pressed\n";
                projectiles.push_back(projectileFactory.createProjectile(player.object->getTransform().transformPoint(player.object->getPoint(2)),
                                                                         player.object->getRotation()));
            }
        }
    // game loop
        if(event.type == sf::Event::KeyPressed) {
            // test if key has been pressed and apply to player's velocity
            player.shipControls(event.key.code);

        }
/*
    info.setString("Velocity: " + std::to_string(player.calcMag()) 
            + "\nRotation: " + std::to_string(player.object->getRotation())
            + "\nX: " + std::to_string(player.calcMag() * std::sin(player.object->getRotation())) 
            + "\nY: " + std::to_string(player.calcMag() * std::cos(player.object->getRotation())));
*/

    player.updateLocation();
    window.clear();
    window.draw(info);
    
    for(auto& asteroid : asteroids) {
        asteroid.updateLocation();
        asteroid.draw(window);
    }
    for(auto& projectile : projectiles) {
        projectile.updateLocation();
        projectile.setAge(projectile.getAge() + 1);
        if(projectile.getAge() > 75) {
            projectiles.erase(projectiles.begin());
        } 

        projectile.draw(window);
    }
    player.draw(window);
    window.display();

    

    }

    delete player.object;
    for(auto& projectile : projectiles) {
        delete projectile.object;
    }

    return 0;
}
