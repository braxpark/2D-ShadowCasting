#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Star.hpp"

class Model {
    public:
        Model(sf::RenderWindow& w) : window(w) {

            sf::ConvexShape testShape;
            std::vector<sf::Vector2f> testShapePoints = {
                sf::Vector2f(400.f, 400.f),
                sf::Vector2f(700.f, 400.f),
                sf::Vector2f(550.f, 500.f),
            };
            testShape.setPointCount(3);
            for(int i = 0; i < 3; i++){
                testShape.setPoint(i, testShapePoints[i]);
            }
            testShape.setFillColor(sf::Color::Green);
            staticShapes.push_back(std::move(testShape));

            sf::ConvexShape convex;
            convex.setPointCount(5);
            convex.setPoint(0, sf::Vector2f(50.f, 50.f));
            convex.setPoint(1, sf::Vector2f(200.f, 60.f));
            convex.setPoint(2, sf::Vector2f(170.f, 140.f));
            convex.setPoint(3, sf::Vector2f(80.f, 150.f));
            convex.setPoint(4, sf::Vector2f(50.f, 100.f));
            staticShapes.push_back(std::move(convex));
        }
        void update(sf::RenderWindow& window, int mouse_x, int mouse_y);
        void draw();
        void handleKeyMovement(sf::Event &event, sf::ConvexShape &shape){
            std::vector<sf::Vector2f> points = getPoints(shape);
            float deltaX = 0.f, deltaY = 0.f;
            switch(event.key.code){
                case sf::Keyboard::Right:
                    deltaX = 5.f;
                    break;
                case sf::Keyboard::Left:
                    deltaX = -5.f;
                    break;
                case sf::Keyboard::Up:
                    deltaY = -5.f;
                    break;
                case sf::Keyboard::Down:
                    deltaY = 5.f;
                    break;
            }
            for(size_t i = 0; i < points.size(); i++){
                points[i].x += deltaX;
                points[i].y += deltaY;
                shape.setPoint(i, points[i]);
            }
        }

        Star star;
        std::vector<sf::ConvexShape> staticShapes;
        sf::RenderWindow& window;
        bool movingStar = false;
};

void Model::update(sf::RenderWindow &window, int mouse_x, int mouse_y){
    sf::Vector2f mouses = sf::Vector2f((float)mouse_x, (float)mouse_y);
    star.update(mouses, staticShapes, window);
}

void Model::draw(){
    for(auto &shape : staticShapes){
        window.draw(shape);
    }
    for(auto &beam : star.beams){
        window.draw(beam);
    }
}