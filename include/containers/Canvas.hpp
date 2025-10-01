#pragma once

namespace myui {
    namespace containers {

    class Canvas : public Container {
        public:

        Canvas(const sf::RenderWindow& window) {
            setSize(sf::Vector2f(window.getSize()));
            e_padding = {0,0};
        }

        void draw(sf::RenderTarget& target) override {
            if(!visible || !enabled) return;

            sf::RectangleShape shape(e_size);
            shape.setPosition(e_position);
            shape.setFillColor(e_scheme.background);
            target.draw(shape);

            passDraw(target);
        }

        bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
            if(!enabled) return false;
            if (event.is<sf::Event::Resized>()){
                setSize(sf::Vector2f(window.getSize()));
            }
            return Container::handleEvent(event, window);
        }

    };

    }
}

