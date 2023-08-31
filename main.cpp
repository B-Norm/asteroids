#include "assets.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <clocale>
#include <iostream>
#include <SFML/Window.hpp>
#include <cmath>
#include <string>


int main () {
    int lvl = 1;
    bool lvlTest = true;
    int totalScore = 0;
    int scoreAdult = 500;
    int scoreChild = 1000;

    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(WindowInfo::windowX, WindowInfo::windowY), "Asteroidz");
    //window.setPosition(sf::Vector2i(700,20));
    window.setFramerateLimit(60);

    // vector for bullets
    std::vector<Projectile> projectiles;
    ProjectileFactory projectileFactory;

    // vector for asteroids
    std::vector<Asteroid> asteroids;
    AsteroidFactory asteroidFactory;

    // load in player
    Player player(WindowInfo::windowX/2, WindowInfo::windowY/2);

    // debug info for player-----------------------------------
    sf::Font font;
    font.loadFromFile("runescape_uf.ttf");
    sf::Text info("Score: ", font, 24);
    info.setFillColor(sf::Color::White);
    info.setPosition(10.f,10.f);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
                projectiles.push_back(projectileFactory.createProjectile(player.object->getTransform().transformPoint(player.object->getPoint(2)),
                                                                         player.object->getRotation()));
            }
        }
    // game loop
        if(event.type == sf::Event::KeyPressed) {
            // test if key has been pressed and apply to player's velocity
            player.shipControls(event.key.code);

        }
        if(lvlTest) {
            for(int i = 0 ; i < lvl + 3 ; i++) {
                asteroids.push_back(asteroidFactory.createAsteroid(true, player.object->getPosition(), rand()));
            } 
            lvl +=1;
            lvlTest = false;
        }

        info.setString("Score: " + std::to_string(totalScore));

        player.updateLocation();

        window.clear();
        window.draw(info);
        int j = 0;
        for(auto& asteroid : asteroids) {
            asteroid.updateLocation();
             
            //TODO: check for collison with more exact mesurments and destroy graphics
            int i = 0;
            for(auto& projectile : projectiles) {
                if(projectile.object->getGlobalBounds().intersects(asteroid.object->getGlobalBounds())) {
                    projectiles.erase(projectiles.begin() + i);

                    if(asteroid.getAdult()) {
                        // spawn two smaller asteroids
                        asteroids.push_back(asteroidFactory.createAsteroid(false, asteroid.object->getPosition(), rand()));
                        asteroids.push_back(asteroidFactory.createAsteroid(false, asteroid.object->getPosition(), rand()));
                        totalScore += (scoreAdult * lvl);
                    } else {
                        totalScore += (scoreChild * lvl);
                    }

                    asteroids.erase(asteroids.begin() + j);
                }
                i++;
            }


            if(player.object->getGlobalBounds().intersects(asteroid.object->getGlobalBounds())) {
                // destroy ship
                return 0;
            }
            asteroid.draw(window);
            j++;
        }

        if(j == 0){
           lvlTest = true;  
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
    for(auto& asteroid : asteroids) {
        delete asteroid.object;
    }
    for(auto& projectile : projectiles) {
        delete projectile.object;
    }

    return 0;
}
