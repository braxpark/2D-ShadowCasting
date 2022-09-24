#pragma once
#include "Model.hpp"
namespace Rays {
    using namespace sf;
    struct App{
        public:
            App(const std::string& title);
            void run();
        private:
            std::string m_Title;
    };

    App::App(const std::string& title)
    {
        m_Title = title;
    }
    
    void App::run()
    {
        RenderWindow window(VideoMode(1850,960), m_Title, sf::Style::Titlebar | sf::Style::Close);
        window.setPosition(Vector2i(0,25));
        Model m(window);
        while(window.isOpen())
        {
            Clock clock;
            Event event;
            while(window.pollEvent(event)){
                switch(event.type)
                {
                    case Event::KeyPressed:
                        m.handleKeyMovement(event);
                        break;
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::Resized:
                        FloatRect visibleArea(0,0,event.size.width, event.size.height);
                        window.setView(View(visibleArea));
                        break;
                }
            }
            Time t1 = clock.getElapsedTime();
            Vector2i mousePos = Mouse::getPosition(window);
            m.update(window, mousePos.x, mousePos.y);
            window.clear();
            m.draw();
            window.display();
            Time t2 = clock.restart();
            std::cout << "Frames Per Second: " << 1.0f / t2.asSeconds() << '\n';
        }
        std::cout << std::endl;
    }
} //End of Namespace Rays