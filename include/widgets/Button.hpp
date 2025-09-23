#pragma once


namespace myui{
    namespace widgets{

    class Button : public Element{

        public:
        Button(sf::Vector2f pos){
            e_position = pos;
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            sf::CircleShape shape(100.f);
            shape.setFillColor(sf::Color::Green);
            shape.setPosition(e_position+e_offset);
            target.draw(shape);
        }

        sf::FloatRect getBounds() override {
            return sf::FloatRect(e_position, e_size);
        }

    };

    };
};