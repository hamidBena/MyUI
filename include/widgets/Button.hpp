#pragma once

namespace myui {
    namespace widgets {

    class Button : public Element{
        public:
        Button(){
            e_sizeType = sizeTypes::absolute;
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            Element::draw(target, states);
            if(e_renderMode == renderMode::SchemeColors){
                drawShape(target, states);
            }
            if(debug) drawDebug(target, states);
        }

        void sizePass() override {
            Element::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                if(e_renderMode == renderMode::SingleTextureTinted){
                    e_size = sf::Vector2f(e_texture->getSize());
                }else{
                    sf::Text label(*e_font, e_label, e_labelSize);
                    auto size = label.getLocalBounds().size;
                    e_size = size + sf::Vector2f(e_padding.x * 4, e_padding.y * 4);
                }
            }
        }

        private:
        void drawShape(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default){
            sf::RectangleShape button(e_size);
            button.setPosition(e_position);

            //color handling
            if(hovered)  button.setFillColor(e_scheme.hover);
            else         button.setFillColor(e_scheme.foreground);   

            if(held)     button.setFillColor(e_scheme.pressed);
            if(!enabled) button.setFillColor(e_scheme.disabled);
            
            //border handling
            if(hovered)  {button.setOutlineColor(e_scheme.accent); button.setOutlineThickness(2);}
            else         {button.setOutlineColor(e_scheme.border); button.setOutlineThickness(1);}

            if(held)     {button.setOutlineColor(e_scheme.border); button.setOutlineThickness(3);}
            if(!enabled) {button.setOutlineColor(e_scheme.disabled); button.setOutlineThickness(1);}
            
            //label handling
            sf::Text label(*e_font, e_label, e_labelSize);
            label.setFillColor(e_scheme.text);
            label.setPosition(e_position + e_padding);

            target.draw(button);
            target.draw(label);
        }
    };

    }
}