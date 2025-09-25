#pragma once

namespace myui {
    namespace containers {

    class Canvas : public Container {
        public:

        Canvas(const sf::RenderWindow& window) {
            e_size = sf::Vector2f(window.getSize());
            e_padding = {0,0};
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            if(!visible || !enabled) return;

            sf::RectangleShape shape(e_size);
            shape.setPosition(e_position);
            shape.setFillColor(e_scheme.background);
            target.draw(shape);

            passDraw(target, states);
        }

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
            if(!enabled) return;
            
            Container::handleEvent(event, window);
            if (event.is<sf::Event::Resized>()){
                e_size = sf::Vector2f(window.getSize());
            }
        }

    };

    }
}

