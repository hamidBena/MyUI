#pragma once

namespace myui {
    namespace containers {

    class HScroll : public Container {
        public:
        HScroll() : bufferSprite(buffer.getTexture()){
            e_padding = {0,0};
        }

        void draw(sf::RenderTarget& target) override {
            if(!visible || !enabled) return;

            buffer = sf::RenderTexture(target.getSize());
            buffer = sf::RenderTexture(sf::Vector2u(e_size));

            Container::draw(target);
            if(e_renderMode == renderMode::SchemeColors){
                sf::RectangleShape shape(intr_size);
                shape.setPosition(intr_position);
                shape.setFillColor(e_scheme.background);
                shape.setOutlineColor(e_scheme.border);
                shape.setOutlineThickness(1);
                target.draw(shape);
            }

            sf::FloatRect clipArea(e_position, e_size);

            // Create a view with that area
            sf::View view(clipArea);
            buffer.setView(view);

            // --- Draw into buffer ---
            buffer.clear(e_scheme.background);
            passDraw(buffer);
            buffer.display();

            // --- Draw buffer as sprite ---
            bufferSprite.setTexture(buffer.getTexture(), true);
            bufferSprite.setPosition(e_position);
            target.draw(bufferSprite);

        }

        bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
            if(auto scroll = event.getIf<sf::Event::MouseWheelScrolled>()){
                if(getBounds().contains(sf::Vector2f(scroll->position))) {
                    scroll->delta > 0 ? scrollOffset += scrollSensitivty : scrollOffset -= scrollSensitivty;

                    if(scrollOffset < lowerScrollBound) scrollOffset = lowerScrollBound;
                    if(scrollOffset > upperScrollBound) scrollOffset = upperScrollBound;

                }
            }

            bool isConsumed = false;
            if(Element::handleEvent(event, window) || hovered){
                for(auto& child : children) 
                    if(child->handleEvent(event, window)) return true;
                isConsumed = true;
            }

            return isConsumed;
        }

        void layoutPass() override {
            Container::layoutPass();
            for(auto& child : children){
                if(!child->enabled || !child->visible) continue;

                if (auto con = dynamic_cast<Container*>(child.get())) {
                    // child is a Container
                    con->moveContainer(con->e_position + sf::Vector2f(scrollOffset, 0));
                } else {
                    // child is a normal widget
                    child->setPosition(child->e_position + sf::Vector2f(scrollOffset, 0));
                }
            }
        }

        void sizePass() override {
            Container::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                float maxHeight = 0;
                for(auto& child : children){
                    if(!child->enabled || !child->visible) continue;
                    if(child->e_size.y > maxHeight) maxHeight = child->e_size.y;
                }
                setSize(sf::Vector2f(width, maxHeight + e_padding.y * 2));
            }

            float contentTop = std::numeric_limits<float>::max();
            float contentBottom = std::numeric_limits<float>::lowest();
            for (auto& child : children) {
                if (!child->enabled || !child->visible) continue;

                float top = child->e_position.x;
                float bottom = child->e_position.x + child->e_size.x;

                if (top < contentTop) contentTop = top;
                if (bottom > contentBottom) contentBottom = bottom;
            }

            // Handle empty content
            if (children.empty() || contentTop == std::numeric_limits<float>::max()) {
                lowerScrollBound = upperScrollBound = 0.0f;
            } else {
                float contentHeight = contentBottom - contentTop;

                lowerScrollBound = width-contentHeight; // you usually scroll from 0
                upperScrollBound = 0;
            }
        }

        public:
        float scrollOffset = 0;
        float scrollSensitivty = 15;
        float width = 0;

        float lowerScrollBound = 0;
        float upperScrollBound = 0;

        sf::RenderTexture buffer;
        sf::Sprite bufferSprite;
        
    };

    }
}