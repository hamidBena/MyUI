#pragma once
#include <functional>
#include <SFML/Graphics.hpp>

namespace myui{

class Element {
public:
    inline static int ElementCount = 0;

    sf::Vector2f e_position = {0, 0};
    sf::Vector2f e_offset   = {0, 0};
    sf::Vector2f e_size     = {150, 50};
    sf::Vector2f e_padding  = {5, 5};

    //Interpolated<sf::Vector2f> intr_position;
    //Interpolated<sf::Vector2f> intr_size;

    bool visible = true;
    bool enabled = true;
    bool hovered = false;
    float hoverDuration = 0;
    const float getHoverDuation() {return hoverDuration;}

    std::function<void(Element&, const float&)> onTick;
    std::function<void(Element&, const float&)> onHover;
    std::function<void(Element&)> onMouseEnter;
    std::function<void(Element&)> onMouseLeave;

    void setOnTick(std::function<void(Element&, const float&)> cb) { onTick = std::move(cb); }
    void setOnHover(std::function<void(Element&, const float&)> cb) { onHover = std::move(cb); }
    void setOnMouseEnter(std::function<void(Element&)> cb) { onMouseEnter = std::move(cb); }
    void setOnMouseLeave(std::function<void(Element&)> cb) { onMouseLeave = std::move(cb); }


    Element* parent;

    virtual void update(const float dt);
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) = 0;
    virtual sf::FloatRect getBounds() = 0;

    virtual ~Element() = default; 
};

class Container : public Element {
public:
    std::vector<std::unique_ptr<Element>> children;

    //override to pass data to children
    void update(const float dt) override {
        Element::update(dt);
        passUpdate(dt);
    }

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
        Element::handleEvent(event, window);
        passEvent(event, window);
    }

    void passUpdate(const float dt) {
        for (auto& child : children)
            child->update(dt);
    }
    void passEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& child : children)
            child->handleEvent(event, window);
    }
    void passDraw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
        for (auto& child : children)
            child->draw(target, states);
    }

};

};