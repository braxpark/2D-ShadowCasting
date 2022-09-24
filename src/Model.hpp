#ifndef MODEL_H
#define MODEL_H

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Star.hpp"

class Model {
    public:
        Model(sf::RenderWindow& w) : window(w) {

            //Creation of demo shapes used in scene
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

            sf::ConvexShape convex2;
            convex2.setPointCount(4);
            convex2.setPoint(0, sf::Vector2f( 1300.f, 350.f));
            convex2.setPoint(1, sf::Vector2f( 1500.f, 450.f));
            convex2.setPoint(2, sf::Vector2f( 1300.f, 550.f));
            convex2.setPoint(3, sf::Vector2f( 1100.f, 450.f));
            staticShapes.push_back(std::move(convex2));
            
        }
        void update(sf::RenderWindow& window, int mouse_x, int mouse_y);
        void draw();
        void handleKeyMovement(sf::Event &event)
        {
            switch(event.key.code){
                case sf::Keyboard::Escape:
                    window.close();
                    break;
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

#endif // ! MODEL_H