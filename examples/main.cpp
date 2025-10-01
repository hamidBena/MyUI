#include "MyUI.hpp"
#include <variant>
#include <chrono>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({2000u, 1200u}), "MyUI Example");
    window.setFramerateLimit(120);

    myui::GUI myUI;
    
    auto canvas = myUI.createContainer<myui::containers::Canvas>(window);

    auto HLayout = canvas->CreateElement<myui::containers::VLayout>();
    HLayout->setOffset({500,200});

    auto scroll1 = HLayout->CreateElement<myui::containers::VScroll>();
    scroll1->height = 100;
    auto scroll2 = HLayout->CreateElement<myui::containers::HScroll>();
    scroll2->width = 220;

    auto buttonList1 = scroll1->CreateElement<myui::containers::VLayout>();
    auto buttonList2 = scroll2->CreateElement<myui::containers::HLayout>();

    auto button1 = buttonList2->CreateElement<myui::widgets::Button>();
    auto button2 = buttonList2->CreateElement<myui::widgets::Button>();
    auto button3 = buttonList2->CreateElement<myui::widgets::Button>();
    auto button4 = buttonList2->CreateElement<myui::widgets::Button>();

    auto bitmap = buttonList1->CreateElement<myui::widgets::BitMap>(10, 10);
    bitmap->setSize({200, 200});

    bitmap->setOnPress([&bitmap](auto& element, auto& duration){
        auto idx = bitmap->getHoverIndex();
        std::cout<<bitmap->hoverX<<", "<<bitmap->hoverY<<std::endl;

        sf::Color colorRND = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
        
        bitmap->setPixel(idx%bitmap->width, idx/bitmap->height, colorRND);
    });



    //canvas->setDebuggingMode(true);

    sf::Clock clock;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                window.close();
            }
                
            myUI.handleEvent(*event, window);
        }

        window.clear(sf::Color::Black);

        float dt = clock.restart().asSeconds();

        myUI.update(dt);
        myUI.draw(window);


        window.display();
    }

    return 0;
}
