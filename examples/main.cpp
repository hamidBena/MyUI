#include "MyUI.hpp"
#include <variant>
#include <iostream>

int main() {
    // Create SFML window (SFML 3.x way)
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "MyUI Example");

    myui::GUI myUI;

    auto button2 = myUI.CreateElement<myui::widgets::Button>(sf::Vector2f(100, 100));
    

    sf::Clock clock;
    while (window.isOpen()) {
        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                window.close();
            }
                
            myUI.handleEvent(*event, window);
        }

        // Clear + draw
        window.clear(sf::Color::Black);

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        myUI.update(dt);
        myUI.draw(window);


        window.display();
    }

    return 0;
}
