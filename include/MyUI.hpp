#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Element.hpp"
#include "utils/env.hpp"
#include "utils/assetManager.hpp"

#include "widgets/widgets.hpp"
#include "containers/containers.hpp"

namespace myui{

class GUI {
public:
    template <typename T, typename... Args>
    T* createContainer(Args&&... args) {
        static_assert(std::is_base_of_v<Container, T>, "T must derive from Container");
        auto container = std::make_unique<T>(std::forward<Args>(args)...);
        auto ptr = container.get();
        elements.push_back(std::move(container));
        return ptr;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
        sf::View oldView = target.getView();
        sf::Vector2u winSize = target.getSize();
        sf::View uiView = sf::View(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(winSize)));
        target.setView(uiView);
        for (auto& element : elements){
            element->layoutPass();
            element->draw(target, states);
        }
        target.setView(oldView);
    }

    void update(const float dt) {
        for (auto& element : elements)
            element->update(dt);
    }
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : elements)
            element->handleEvent(event, window);
    }

private:
    std::vector<std::unique_ptr<Container>> elements;

};

};
