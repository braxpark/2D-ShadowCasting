#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

const double pi = std::atan(1)*4;
const double radian_factor = (pi / 2) / 90; // this * theta == theta'  (AKA thetaInRadians -- in radians from degrees)

struct _vec2f { 
    _vec2f() {}
    _vec2f(const float a, const float b) {
        x = a;
        y = b;
    }
    float x, y;
 };
typedef _vec2f vec2f;

_vec2f negate(_vec2f a) {
    return(_vec2f(a.x * -1.0f, a.y * -1.0f));
}

size_t dot(_vec2f a, _vec2f b) {
    size_t result = 0.0f;
    result+=(a.x * b.x);
    result+=(a.y * b.y);
    return result;
}

_vec2f support(const sf::RectangleShape p, _vec2f direction) {

}

_vec2f subtract(_vec2f a, _vec2f b) {
    return(_vec2f(a.x - b.x, a.y - b.y));
}


struct Beam {
    sf::RectangleShape body;
    sf::Vector2f size, position;
    Beam(float angle, sf::Vector2f starPosition) {
        size = sf::Vector2f(50.0, 0.0);
        position = starPosition;
        body.setSize(size);
        body.setPosition(position);
        body.setRotation(angle);
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Green);
        body.setOutlineThickness(1.0);
    }

    void update(sf::Vector2u windowSize, sf::Vector2f newStarPosition){
        position = newStarPosition;
        body.setPosition(position); //source of light
        bool colliding = false;
        sf::Vector2f edge = sf::Vector2f(0.0, 0.0);
        int idx = 0;
        bool xFlag = true, yFlag = false;
        float copy;
        while(!colliding){
            //calculate border collision and wall collision
            //calculate x and y components for +1 x length on body.size
            float theta = body.getRotation(), thetaInRadians = (float)radian_factor * theta;
            edge.x+=1.0; // edge.x == hypotenus
            float xComp = cos(thetaInRadians) * edge.x, yComp = sin(thetaInRadians) * edge.x; // xComponent -- yComponent via trig
            float adjX = position.x + xComp, adjY = position.y + yComp;
            bool xWorks = (adjX > 0 and adjX < windowSize.x), yWorks = (adjY > 0 and adjY < windowSize.y);
            bool wallsWork = true;
            if(xWorks and yWorks and wallsWork){
                continue;
            }
            else{
                edge.x-=1.0;
                colliding = true;
            }
        }
        body.setSize(edge);
    }
    void draw(sf::RenderWindow &window){
        window.draw(body);
    }
    
    bool isColliding(sf::RectangleShape object){
        //  GJK 

        /*
            1. iteration = 0, simplex set = {}, let v0 be arbitrary point (center of gravity?)
            2. Compute the support point
        */

    }

};

struct Wall {
    sf::RectangleShape body;
    Wall(sf::Vector2f position, sf::Vector2f size, float angle){
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::White);
        body.setOutlineThickness(2);
        body.setSize(size);
        body.setPosition(position);
        body.setRotation(angle);
    }
};

class Star {
    public:
        std::vector<Beam> beams;
        sf::RenderWindow &window;
        int numberOfRays = 128;
        sf::Vector2f starPosition = sf::Vector2f(100.0,100.0);

        Star(sf::RenderWindow &w) : window(w) {
            float angleFactor = (float)360 / (float)numberOfRays;
            for(int i = 0; i < numberOfRays; i++){
                beams.emplace_back(Beam((float)i * angleFactor, starPosition));
            }
        }
        void update(sf::Vector2f newStarPosition){
            starPosition = newStarPosition;
            for(auto &beam : beams){
                beam.update(window.getSize(), starPosition);
            }
        }
        void draw(){
            for(auto &beam : beams){
                beam.draw(window);
            }
        }
};