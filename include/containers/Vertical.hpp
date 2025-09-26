#pragma once

namespace myui {
    namespace containers {

    class VLayout : public Container {
        public:
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            if(!visible || !enabled) return;

            Container::draw(target, states);
            if(e_renderMode == renderMode::SchemeColors){
                sf::RectangleShape shape(intr_size);
                shape.setPosition(intr_position);
                shape.setFillColor(e_scheme.background);
                shape.setOutlineColor(e_scheme.border);
                shape.setOutlineThickness(1);
                target.draw(shape);
            }
            passDraw(target, states);
        }

        void layoutPass() override {
            Container::layoutPass();
            float y = 0;
            for(auto& child : children){
                if(!child->enabled || !child->visible) continue;
                child->setPosition(sf::Vector2f(e_position.x, e_position.y + y) + e_padding);
                y += child->e_size.y + spacing;
                child->sizePass();
            }
            sizePass();
        }

        void sizePass() override {
            Container::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                float maxWidth = 0;
                float height = 0;
                for(auto& child : children){
                    if(!child->enabled || !child->visible) continue;
                    if(child->e_size.x > maxWidth) maxWidth = child->e_size.x;
                    height += child->e_size.y + spacing;
                }

                setSize(sf::Vector2f(maxWidth + e_padding.x * 2, height + e_padding.y * 2 - spacing));
            }
        }

        float spacing = 10;
    };

    }
}

