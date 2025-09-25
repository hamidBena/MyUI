#include "Element.hpp"

namespace myui{

    void Element::update(const float dt) {
        if (onTick) onTick(*this, dt);

        if (hovered) {
            hoverDuration += dt;
            if (onHover) onHover(*this, hoverDuration);
        } else {
            hoverDuration = 0;
        }

        if(held){
            heldDuration += dt;
            if(onHeld) onHeld(*this, heldDuration);
        }else{
            heldDuration = 0;
        }
    }

    void Element::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if(!enabled) return;

        if (event.is<sf::Event::MouseMoved>()) {
            if (auto* data = event.getIf<sf::Event::MouseMoved>()) {
                if (!hovered && getBounds().contains(sf::Vector2f(data->position))) {
                    hovered = true;
                    if (onMouseEnter) onMouseEnter(*this);
                } else if (hovered && !getBounds().contains(sf::Vector2f(data->position))) {
                    hovered = false;
                    if (onMouseLeave) onMouseLeave(*this);
                }
            }
        }

        if(auto click = event.getIf<sf::Event::MouseButtonPressed>()){
            if(click->button == sf::Mouse::Button::Left && getBounds().contains(sf::Vector2f(click->position))){
                held = true;
            }
        }
        
        if(auto release = event.getIf<sf::Event::MouseButtonReleased>()){
            if(release->button == sf::Mouse::Button::Left && held){
                if(getBounds().contains(sf::Vector2f(release->position))){
                    if(onClick) onClick(*this, heldDuration);
                }

                if(onRelease) onRelease(*this, heldDuration);

                held = false;
            }
        }
    }

    void Element::draw(sf::RenderTarget& target, sf::RenderStates states) {
        if((e_renderMode == renderMode::SchemeColors || e_renderMode == renderMode::SingleTextureTinted) && e_texture == nullptr) return;

        if(e_renderMode == renderMode::SingleTextureTinted) {
            sf::Sprite sprite(*e_texture);

            sf::FloatRect rect = getBounds();
            sf::Vector2u texSize = e_texture->getSize();

            float texW = static_cast<float>(texSize.x);
            float texH = static_cast<float>(texSize.y);

            // Scale texture to fit rect
            float scaleX = rect.size.x  / texW;
            float scaleY = rect.size.y / texH;

            sprite.setScale(sf::Vector2f(scaleX, scaleY));
            sprite.setPosition(rect.position);

            if(hovered) sprite.setColor(e_tintScheme.hover);
            else sprite.setColor(e_tintScheme.foreground);

            if(held) sprite.setColor(e_tintScheme.pressed);
            if(!enabled) sprite.setColor(e_tintScheme.disabled);

            target.draw(sprite);
        }
    }

    /*
    each element can have its bound & sizepass functions to decide its components size to fit its content
    */

    void Element::layoutPass() {
        if(parent)
            e_position = parent->e_position + parent->e_padding + e_offset;
        else 
            e_position = e_offset;

        sizePass();
    }

    void Element::drawDebug(sf::RenderTarget& target, sf::RenderStates states) {
        static sf::Color hover = {0, 0, 200, 100};
        static sf::Color disable = {200, 0, 0, 100};
        static sf::Color idle = sf::Color::Transparent;
        //hitbox info
        sf::RectangleShape hitbox(getBounds().size);
        hitbox.setPosition(e_position);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(1);

        //state info
        if(hovered) hitbox.setFillColor(hover);
        else if(!enabled) hitbox.setFillColor(disable);
        else hitbox.setFillColor(idle);
        target.draw(hitbox);
    }

};