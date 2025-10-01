#include "MyUI.hpp"
#include <variant>
#include <chrono>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({2000u, 1200u}), "MyUI Example");
    window.setFramerateLimit(120);

    myui::GUI myUI;
    
    auto canvas = myUI.createContainer<myui::containers::Canvas>(window);
    canvas->setPadding({10,10});

    auto bitmap = canvas->CreateElement<myui::widgets::BitMap>(100, 100);
    bitmap->setOffset({525,50});
    bitmap->setSize({950,950});

    std::vector<unsigned char> data(bitmap->getDataSize());

    auto buttonList = canvas->CreateElement<myui::containers::VLayout>();

    auto randomize = buttonList->CreateElement<myui::widgets::Button>();
    randomize->setLabel("randomize");
    randomize->setOnClick([&bitmap, &data](auto& element, auto& duration){
        for (size_t i = 0; i < data.size(); i++) {
            data[i] = static_cast<unsigned char>(rand() % 256);
        }
        bitmap->setData(data);
    });

    auto clear = buttonList->CreateElement<myui::widgets::Button>();
    clear->setLabel("clear");
    clear->setOnClick([&bitmap, &data](auto& element, auto& duration){
        bitmap->clear({50,50,50, 150});
        data = bitmap->pixels;
    });

    bitmap->setOnPress([&bitmap](myui::Element& element, const float& duration){
        auto idx = bitmap->getHoverIndex();
        sf::Color randomColor = sf::Color(rand() % 256, rand() % 256, rand() % 256, rand() % 256);
        bitmap->setPixel(idx%bitmap->width, idx/bitmap->width, randomColor);
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
