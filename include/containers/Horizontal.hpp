#pragma once

namespace myui {
    namespace containers {

    class HLayout : public Container {
        public:
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            if(!visible || !enabled) return;

            Container::draw(target, states);
            if(e_renderMode == renderMode::SchemeColors){
                sf::RectangleShape shape(e_size);
                shape.setPosition(e_position);
                shape.setFillColor(e_scheme.background);
                shape.setOutlineColor(e_scheme.border);
                shape.setOutlineThickness(1);
                target.draw(shape);
            }
            passDraw(target, states);
        }

        void layoutPass() override {
            Container::layoutPass();
            float h = 0;
            for(auto& child : children){
                if(!child->enabled || !child->visible) continue;
                child->e_position = sf::Vector2f(e_position.x + h, e_position.y) + e_padding;
                h += child->e_size.x + spacing;
                child->sizePass();
            }
            sizePass();
        }

        void sizePass() override {
            Container::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                float maxHeight = 0;
                float width = 0;
                for(auto& child : children){
                    if(!child->enabled || !child->visible) continue;
                    if(child->e_size.y > maxHeight) maxHeight = child->e_size.y;
                    width += child->e_size.x + spacing;
                }
                e_size.y = maxHeight + e_padding.y * 2;
                e_size.x = width + e_padding.x * 2 - spacing; // -spacing to remove the last extra spacing added at the last ranged loop iteration
            }
        }

        float spacing = 10;
    };

    }
}

