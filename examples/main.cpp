#include "MyUI.hpp"
#include <variant>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({2000u, 1200u}), "MyUI Example");
    window.setFramerateLimit(60);

    myui::GUI myUI;
    
    auto canvas = myUI.createContainer<myui::containers::Canvas>(window);
    canvas->setPadding({10, 10});

    auto button1 = canvas->CreateElement<myui::widgets::Button>();
    button1->setOffset({0, 80});
    button1->setSizeType(myui::sizeTypes::fitContent);

    auto label1 = canvas->CreateElement<myui::widgets::Label>();
    label1->setLabel("hello this is a labe!\ndoes the multi line work? lets test it out :)");

    auto checkbox1 = canvas->CreateElement<myui::widgets::CheckBox>();
    checkbox1->setOffset({10, 150});

    checkbox1->setOnToggle([&canvas](myui::Element& widget, auto& boolean){
        if(boolean) canvas->setScheme(myui::DefaultSchemes::light());
        else        canvas->setScheme(myui::DefaultSchemes::dark());
    });

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

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        myUI.update(dt);
        myUI.draw(window);


        window.display();
    }

    return 0;
}
