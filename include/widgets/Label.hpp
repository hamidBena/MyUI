#pragma once

namespace myui {
    namespace widgets {

    class Label : public Element{
        public:
        Label(){
            e_sizeType = sizeTypes::fitContent;
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            Element::draw(target, states);
            
            drawShape(target, states);
            
            if(debug) drawDebug(target, states);
        }

        void sizePass() override {
            Element::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                sf::Text label(*e_font, e_label, e_labelSize);
                auto size = label.getLocalBounds().size;
                setSize(size + sf::Vector2f(e_padding.x * 4, e_padding.y * 4));
            }
        }

        private:
        void drawShape(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default){
            //label handling
            sf::Text label(*e_font, e_label, e_labelSize);
            label.setFillColor(e_scheme.text);
            label.setPosition(intr_position.getValue() + e_padding);
            target.draw(label);
        }
    };

    }
}