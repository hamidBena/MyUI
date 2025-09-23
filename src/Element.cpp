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
    }

};