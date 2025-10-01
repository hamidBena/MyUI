#pragma once

namespace myui {
    namespace containers {

    class HLayout : public Container {
        public:
        void draw(sf::RenderTarget& target) override {
            if(!visible || !enabled) return;

            Container::draw(target);
            if(e_renderMode == renderMode::SchemeColors){
                sf::RectangleShape shape(intr_size);
                shape.setPosition(intr_position);
                shape.setFillColor(e_scheme.background);
                shape.setOutlineColor(e_scheme.border);
                shape.setOutlineThickness(1);
                target.draw(shape);
            }
            passDraw(target);
        }

        void layoutPass() override {
            Container::layoutPass();
            float h = 0;
            for(auto& child : children){
                if(!child->enabled || !child->visible) continue;
                if (auto con = dynamic_cast<Container*>(child.get())) {
                    // child is a Container
                    con->moveContainer(sf::Vector2f(e_position.x + h, e_position.y) + e_padding);
                } else {
                    // child is a normal widget
                    child->setPosition(sf::Vector2f(e_position.x + h, e_position.y) + e_padding);
                }
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

                setSize(sf::Vector2f(width + e_padding.x * 2 - spacing, maxHeight + e_padding.y * 2));
            }
        }

        float spacing = 10;
    };

    }
}

