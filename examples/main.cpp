#include "MyUI.hpp"
#include <variant>
#include <chrono>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({2000u, 1200u}), "MyUI Example");
    window.setFramerateLimit(120);

    myui::GUI myUI;
    
    auto canvas = myUI.createContainer<myui::containers::Canvas>(window);

    auto HLayout = canvas->CreateElement<myui::containers::HLayout>();
    HLayout->setOffset({500,200});

    auto sideButtonsScroll = HLayout->CreateElement<myui::containers::VScroll>();
    sideButtonsScroll->height = 600;
    auto sideButtons = sideButtonsScroll->CreateElement<myui::containers::VLayout>();

    auto Panel2 = HLayout->CreateElement<myui::containers::VLayout>();
    auto bitmap = Panel2->CreateElement<myui::widgets::BitMap>(120, 120);
    bitmap->setOffset({10,10});
    auto scroll2 = Panel2->CreateElement<myui::containers::HScroll>();
    scroll2->width = 500;
    
    auto buttonList2 = scroll2->CreateElement<myui::containers::HLayout>();

    auto RightScroll = HLayout->CreateElement<myui::containers::VScroll>();
    RightScroll->height = 600;
    auto RightPanel = RightScroll->CreateElement<myui::containers::VLayout>();

    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();
    RightPanel->CreateElement<myui::widgets::CheckBox>();

    auto button1 = buttonList2->CreateElement<myui::widgets::Button>();
    button1->setLabel("Clear");
    auto button2 = buttonList2->CreateElement<myui::widgets::Button>();
    button2->setLabel("Reset");
    auto button3 = buttonList2->CreateElement<myui::widgets::Button>();
    button3->setLabel("Randomize");
    auto button4 = buttonList2->CreateElement<myui::widgets::Button>();
    button4->setLabel("Fill");


    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();    
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();
    sideButtons->CreateElement<myui::widgets::Button>();

    
    bitmap->setSize({500, 500});

    button1->setOnClick([&bitmap](auto& element, auto& duration){
        bitmap->clearData({200,200,200,150});
    });

    bitmap->setOnPress([&bitmap](auto& element, auto& duration){
        auto idx = bitmap->getHoverIndex();
        sf::Color colorRND = sf::Color(rand() % 100 + 155, 0, 0, 255);
        bitmap->setPixel(idx%bitmap->width, idx/bitmap->height, colorRND);
    });

    canvas->setScheme(myui::DefaultSchemes::light());
    
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
