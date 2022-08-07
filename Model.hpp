#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Star.hpp"

enum class Sort{None, Merge};

class Model {
    public:
        Model(sf::RenderWindow& w) : window(w) {
            stars.emplace_back(Star(window));
	    }
        void update(sf::RenderWindow& window, int mouse_x, int mouse_y);
        void draw();
        std::vector<sf::RectangleShape> rays;   
        sf::RenderWindow& window;
        std::vector<Star> stars;
        bool movingStar = false;
};


void Model::update(sf::RenderWindow &window, int mouse_x, int mouse_y){
    sf::Vector2f mouses = sf::Vector2f((float)mouse_x, (float)mouse_y);
    //std::cout << mouses.x << " " << mouses.y << "\n";
    if(movingStar)
        stars[0].update(mouses);
    else
        stars[0].update(stars[0].starPosition);

}

void Model::draw(){
    stars[0].draw();
}