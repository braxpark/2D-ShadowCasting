#include "Model.hpp"
#include <iostream>


class Controller{

    public:
        Controller(Model& m) : model(m){
        }
        
        void updateEvent(sf::Event &event);
        void update(sf::RenderWindow &window);
        
        int delta_time;
    private:
        Model& model;

};

void Controller::update(sf::RenderWindow &window){

    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    this->model.update(window, mouse_position.x, mouse_position.y);

}

void Controller::updateEvent(sf::Event &event){
    int x = event.mouseButton.x, y = event.mouseButton.y;
    if(event.type == sf::Event::MouseButtonPressed){
        if(event.mouseButton.button == sf::Mouse::Left){
        }
        else if(event.mouseButton.button == sf::Mouse::Right){
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased){
        if(event.mouseButton.button == sf::Mouse::Left){
        }
        else if(event.mouseButton.button == sf::Mouse::Right){
        }
    }
    else if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::B){
        }
        else if(event.key.code == sf::Keyboard::S){
        }
        else if(event.key.code == sf::Keyboard::G){
        }
        else if(event.key.code == sf::Keyboard::W){
        }
        else if(event.key.code == sf::Keyboard::R){
        }
        else if(event.key.code == sf::Keyboard::Space){
        }
        else if(event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down){
        }
    }
    else if(event.type == sf::Event::MouseWheelScrolled){
        if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
            //pos is up scroll
            //negative is down scroll
            if(event.mouseWheelScroll.delta == 1){
            }
            else if(event.mouseWheelScroll.delta == -1){
            }
        }
    }
}
