#pragma once
#include <vector>

namespace myui {
    namespace widgets {

    class BitMap : public Element{
        public:
        BitMap(unsigned x, unsigned y) : width(x), height(y), pixels(x * y * 4, 0), texture(sf::Vector2u(x, y)), sprite(texture) {
            e_sizeType = sizeTypes::absolute;
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            if(e_renderMode == renderMode::SchemeColors){
                drawShape(target, states);
            }
        }

        bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
            if (hovered){
                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window)) - e_position;
                hoverX = static_cast<unsigned int>(pos.x / (e_size.x / width));
                hoverY = static_cast<unsigned int>(pos.y / (e_size.y / height));
            }
            return Element::handleEvent(event, window);
        }

        void update(const float dt) override {
            Element::update(dt);
            texture.update(&pixels[0], sf::Vector2u(width, height), sf::Vector2u(0,0));
        }


        public:
        unsigned int width;
        unsigned int height;
        std::vector<unsigned char> pixels;
        sf::Texture texture;
        sf::Sprite sprite;

        unsigned int hoverX = 0;
        unsigned int hoverY = 0;

        void setPixel(unsigned x, unsigned y, sf::Color c) {
            if (x >= width || y >= height) return;
            size_t idx = (y * width + x) * 4;
            pixels[idx + 0] = c.r;
            pixels[idx + 1] = c.g;
            pixels[idx + 2] = c.b;
            pixels[idx + 3] = c.a;
        }

        void setData(const std::vector<unsigned char>& data){
            if (data.size() != width * height * 4) return;
            pixels = data;
        }

        size_t getDataSize(){ return width * height * 4; }
        size_t getHoverIndex(){ return (hoverY * width + hoverX); }
        size_t getPixelIndex(const int& x, const int& y){ return (y * width + x); }

        void clearData(const sf::Color& color){
            for (size_t i = 0; i < pixels.size(); i++) {
                if(i % 4 == 0)  pixels[i] = color.r;
                else if(i % 4 == 1)  pixels[i] = color.g;
                else if(i % 4 == 2)  pixels[i] = color.b;
                else if(i % 4 == 3)  pixels[i] = color.a;
            }
        }

        private:
        void drawShape(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
            if (texture.getSize().x == 0 || texture.getSize().y == 0) return;

            // Draw the bitmap normally scaled
            sprite.setScale(sf::Vector2f(
                e_size.x / texture.getSize().x,
                e_size.y / texture.getSize().y
            ));
            sprite.setPosition(intr_position);
            target.draw(sprite, states);

            // Draw grid lines procedurally over the bitmap
            sf::VertexArray lines(sf::PrimitiveType::Lines);

            unsigned int cols = static_cast<unsigned int>(e_size.x);
            unsigned int rows = static_cast<unsigned int>(e_size.y);

            float cellWidth  = e_size.x / width;
            float cellHeight = e_size.y / height;

            // Vertical lines
            for (unsigned int x = 0; x <= width; x++) {
                float px = x * cellWidth + sprite.getPosition().x;
                lines.append(sf::Vertex({px, sprite.getPosition().y}, sf::Color::Black));
                lines.append(sf::Vertex({px, sprite.getPosition().y + e_size.y}, sf::Color::Black));
            }

            // Horizontal lines
            for (unsigned int y = 0; y <= height; y++) {
                float py = y * cellHeight + sprite.getPosition().y;
                lines.append(sf::Vertex({sprite.getPosition().x, py}, sf::Color::Black));
                lines.append(sf::Vertex({sprite.getPosition().x + e_size.x, py}, sf::Color::Black));
            }

            target.draw(lines, states);
        }


    };

    }
}