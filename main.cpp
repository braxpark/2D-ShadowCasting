#include "Controller.hpp"

namespace Default {
    using namespace sf;
    struct App{
        public:
            void run(std::string title);
    };
    void App::run(std::string title){
        RenderWindow window(VideoMode(1850 / 2,960), title);
        window.setPosition(Vector2i(0,50));
        Model m(window);
        Controller c(m);
        while(window.isOpen()){
            Clock clock;
            Event event;
            while(window.pollEvent(event)){
                switch(event.type){
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::Resized:
                        FloatRect visibleArea(0,0,event.size.width, event.size.height);
                        window.setView(View(visibleArea));
                }
                c.updateEvent(event);
            }
            Time t1 = clock.getElapsedTime();
            c.update(window);
            window.clear();
            m.draw();
            window.display();
            Time t2 = clock.restart();
        }
    }
}

int main(){
    Default::App application;
    std::string APP_TITLE = "Sorting!";
    application.run(APP_TITLE);
    return 0;
}
