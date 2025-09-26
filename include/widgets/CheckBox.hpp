#pragma once

namespace myui {
    namespace widgets {

    class CheckBox : public Element{
        public:
        CheckBox(){
            e_sizeType = sizeTypes::fitContent;
            e_labelSize = 18;

            if(e_scheme == DefaultSchemes::dark()){checkMark = std::make_unique<sf::Texture>(AssetManager::get().getTexture("textures/CheckMark2.png"));
                                             }else{checkMark = std::make_unique<sf::Texture>(AssetManager::get().getTexture("textures/CheckMark.png"));}
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override {
            Element::draw(target, states);
            if(e_renderMode == renderMode::SchemeColors){
                drawShape(target, states);
            }

            //check mark
            if(checked) {
                drawCheckMark(target, states);
            }

            if(debug) drawDebug(target, states);
        }

        bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
            if (auto release = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (release->button == sf::Mouse::Button::Left && held) {
                    if (getBounds().contains(sf::Vector2f(release->position))) {
                        checked = !checked; // toggle state
                        if (onToggle) onToggle(*this, checked);
                    }
                }
            }

            return Element::handleEvent(event, window);
        }

        // widget optional overrides
        void sizePass() override {
            Element::sizePass();
            if(e_sizeType == sizeTypes::fitContent){
                sf::Text label(*e_font, e_label, e_labelSize);
                auto size = label.getLocalBounds().size;
                setSize(size + sf::Vector2f(40 + e_padding.x, 0));
                if(e_size.y < 25 ) e_size.y = 25;
            }

            outerBox.size = e_size;
            innerBox.size = {25, 25};

            outerBox.position = intr_position;
            innerBox.position = intr_position;

        }

        sf::FloatRect getBounds() override {
            return innerBox;
        }

        void setScheme(const ColorScheme& scheme) override {
            Element::setScheme(scheme);
            if(e_scheme == DefaultSchemes::dark()){
                checkMark = std::make_unique<sf::Texture>(AssetManager::get().getTexture("textures/CheckMark2.png"));
            }else{
                checkMark = std::make_unique<sf::Texture>(AssetManager::get().getTexture("textures/CheckMark.png"));
            }
        }

        bool isChecked() {return checked;}
        void setChecked(bool chk) {checked = chk;}
        void setOnToggle(std::function<void(CheckBox&, bool&)> cb) { onToggle = std::move(cb); }

        void setChkTexture(std::unique_ptr<sf::Texture> texture){checkMark = std::move(texture);}
        void setBGTexture(std::unique_ptr<sf::Texture> texture){BGTexture = std::move(texture);}


    private:
        //state
        bool checked = false;
        std::function<void(CheckBox&, bool&)> onToggle;
        
        //textures
        std::unique_ptr<sf::Texture> checkMark;
        std::unique_ptr<sf::Texture> BGTexture;


        //hit boxes
        sf::FloatRect innerBox;
        sf::FloatRect outerBox;

        void drawShape(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default){
            sf::RectangleShape box(innerBox.size);
            sf::RectangleShape body(sf::Vector2f(outerBox.size));

            box.setPosition(intr_position);

            body.setPosition(intr_position);
            body.setFillColor(e_scheme.foreground);
            body.setOutlineColor(e_scheme.border);
            body.setOutlineThickness(1);

            //color handling
            if(hovered)  box.setFillColor(e_scheme.hover);
            else         box.setFillColor(e_scheme.foreground);

            if(held)     box.setFillColor(e_scheme.pressed);
            if(!enabled) box.setFillColor(e_scheme.disabled);
            
            //border handling
            if(hovered)  {box.setOutlineColor(e_scheme.accent); box.setOutlineThickness(2);}
            else         {box.setOutlineColor(e_scheme.border); box.setOutlineThickness(1);}

            if(held)     {box.setOutlineColor(e_scheme.border); box.setOutlineThickness(3);}
            if(!enabled) {box.setOutlineColor(e_scheme.disabled); box.setOutlineThickness(1);}
            
            //label handling
            sf::Text label(*e_font, e_label, e_labelSize);
            label.setFillColor(e_scheme.text);
            label.setPosition(intr_position.getValue() + e_padding + sf::Vector2f(30, -7.5));

            target.draw(body);
            target.draw(box);
            target.draw(label);
        }

        void drawCheckMark(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default){
            sf::Sprite mark(*checkMark);

            sf::Vector2u texSize = checkMark->getSize();

            float texW = static_cast<float>(texSize.x);
            float texH = static_cast<float>(texSize.y);

            // Scale texture to fit rect
            float scaleX = innerBox.size.x  / texW;
            float scaleY = innerBox.size.y / texH;

            mark.setScale(sf::Vector2f(scaleX, scaleY));
            mark.setPosition(innerBox.position);

            if(hovered) mark.setColor(e_tintScheme.hover);
            else mark.setColor(e_tintScheme.foreground);

            if(held) mark.setColor(e_tintScheme.pressed);
            if(!enabled) mark.setColor(e_tintScheme.disabled);

            target.draw(mark);
        }
    };

    }
}