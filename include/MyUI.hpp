#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Element.hpp"
#include "widgets/Button.hpp"

namespace myui{


class GUI {
public:
    //std::shared_ptr<UIRoot> CreateRoot();

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateElement(Args&&... args) {
        static_assert(std::is_base_of_v<Element, T>, "T must derive from Element");
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T* createContainer(Args&&... args) {
        static_assert(std::is_base_of_v<Container, T>, "T must derive from Container");
        auto container = CreateElement<T>(std::forward<Args>(args)...);
        auto ptr = container.get();
        elements.push_back(std::move(container));
        return ptr;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
        for (auto& element : elements)
            element->draw(target, states);
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
    std::vector<std::unique_ptr<Element>> elements;

};

};
