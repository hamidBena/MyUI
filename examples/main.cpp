#include "MyUI.hpp"
#include <variant>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({2000u, 1200u}), "MyUI Example");
    window.setFramerateLimit(60);

    myui::GUI myUI;
    
    auto canvas = myUI.createContainer<myui::containers::Canvas>(window);

    auto Vlayout = canvas->CreateElement<myui::containers::VLayout>();

    auto Hlayout = Vlayout->CreateElement<myui::containers::HLayout>();
    //Hlayout->setEnabled(false);

    //Vlayout->setRenderMode(myui::renderMode::SingleTextureTinted);
    //Vlayout->setTexture(std::make_unique<sf::Texture>(myui::AssetManager::get().getTexture("textures/CheckMark.png")));

    auto button1 = Hlayout->CreateElement<myui::widgets::Button>();
    button1->setOffset({0, 80});
    button1->setSizeType(myui::sizeTypes::fitContent);

    auto label1 = Vlayout->CreateElement<myui::widgets::Label>();
    label1->setLabel("hello this is a labe!\ndoes the multi line work? lets test it out :)");

    auto checkbox1 = Hlayout->CreateElement<myui::widgets::CheckBox>();
    checkbox1->setOffset({10, 150});

    Vlayout->setOnClick([canvas](myui::Element& widget, const float& dt){
        widget.e_offset.x += 200 * dt;
    });

    button1->setOnClick([Vlayout](myui::Element& widget, const float& dt){
        Vlayout->spacing += 10;
    });

    checkbox1->setOnToggle([&canvas](myui::Element& widget, auto& boolean){
        if(boolean) canvas->setScheme(myui::DefaultSchemes::light());
        else        canvas->setScheme(myui::DefaultSchemes::dark());
    });

    canvas->setDebuggingMode(true);

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
