#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "styles.hpp"
#include "utils/assetManager.hpp"
#include "utils/Interpolation.hpp"

namespace myui{

    enum class sizeTypes {
        absolute,
        fitContent
    };
    enum class renderMode {
        SchemeColors,       // draw with shapes/colors
        SingleTextureTinted, // one texture + tint
    };

    class Element {
    public:
    inline static int ElementCount = 0;

    public:
    //layout
    sf::Vector2f e_position = {0, 0};
    sf::Vector2f e_offset   = {0, 0};                         
    sf::Vector2f e_size     = {150, 50};                      
    sf::Vector2f e_padding  = {10, 10};                         
    sizeTypes e_sizeType = sizeTypes::fitContent;             

    //style
    ColorScheme e_scheme = DefaultSchemes::dark();            
    ColorScheme e_tintScheme = DefaultSchemes::TextureTints();
    renderMode e_renderMode = renderMode::SchemeColors;       

    //animation
    Interpolated<sf::Vector2f> intr_position;
    Interpolated<sf::Vector2f> intr_size;

    //state
    bool visible = true;
    bool enabled = true;
    bool hovered = false;
    float hoverDuration = 0;
    bool held = false;
    float heldDuration = 0;

    //graphics
    std::unique_ptr<sf::Texture> e_texture;
    std::string e_label = "Label";
    float e_labelSize = 24;
    std::unique_ptr<sf::Font> e_font = std::make_unique<sf::Font>(AssetManager::get().getFont("fonts/arial.ttf"));

    //callbacks
    std::function<void(Element&, const float&)> onTick;
    std::function<void(Element&, const float&)> onHover;
    std::function<void(Element&)> onMouseEnter;
    std::function<void(Element&)> onMouseLeave;
    std::function<void(Element&, const float&)> onClick;
    std::function<void(Element&, const float&)> onHeld;
    std::function<void(Element&, const float&)> onRelease;

    //debugging 
    bool debug = false;

    public:
        Element& setPosition(const sf::Vector2f& position){e_position = position;
                                         intr_position = position; return *this;}                   sf::Vector2f getPosition() {return e_position;}

        Element& setOffset(const sf::Vector2f& offset){e_offset = offset; return *this;}            sf::Vector2f getOffset() {return e_offset;}
        Element& setSize(const sf::Vector2f& size){e_size = size; intr_size = size; return *this;}  sf::Vector2f getSize() {return e_size;}
        Element& setPadding(const sf::Vector2f& padding){e_padding = padding; return *this;}        sf::Vector2f getPadding() {return e_padding;}
        Element& setSizeType(const sizeTypes& type){e_sizeType = type; return *this;}               sizeTypes getSizeType() {return e_sizeType;}
        virtual void setScheme(const ColorScheme& scheme){e_scheme = scheme;}                       ColorScheme getScheme() {return e_scheme;}
        void setTintScheme(const ColorScheme& scheme){e_tintScheme = scheme;}                       ColorScheme getTintScheme() {return e_tintScheme;}
        void setRenderMode(const renderMode& mode){e_renderMode = mode;}                            renderMode getRenderMode() {return e_renderMode;}
        void setVisible(bool visible){this->visible = visible;}                                     bool isVisible() {return visible;}
        void setEnabled(bool enabled){this->enabled = enabled;}                                     bool isEnabled() {return enabled;}
        void setLabel(const std::string& label){e_label = label;}                                   std::string getLabel() {return e_label;}
        void setLabelSize(float size){e_labelSize = size;}                                          float getLabelSize() {return e_labelSize;}
        void setFont(std::unique_ptr<sf::Font> font){e_font = std::move(font);}                     sf::Font* getFont() {return e_font.get();}
        void setTexture(std::unique_ptr<sf::Texture> texture){e_texture = std::move(texture);}      sf::Texture* getTexture() {return e_texture.get();}
        virtual void setDebuggingMode(bool bug){ debug = bug; }                                     bool isDebuggingMode() {return debug;}

        bool isHovered() {return hovered;}
        bool isHeld() {return held;}
        float getHeldDuration() {return heldDuration;}
        float getHoverDuration() {return hoverDuration;}

        void setOnTick(std::function<void(Element&, const float&)> cb) { onTick = std::move(cb); }
        void setOnHover(std::function<void(Element&, const float&)> cb) { onHover = std::move(cb); }
        void setOnMouseEnter(std::function<void(Element&)> cb) { onMouseEnter = std::move(cb); }
        void setOnMouseLeave(std::function<void(Element&)> cb) { onMouseLeave = std::move(cb); }
        void setOnPress(std::function<void(Element&, const float&)> cb) { onHeld= std::move(cb); }
        void setOnRelease(std::function<void(Element&, const float&)> cb) { onRelease = std::move(cb); }
        void setOnClick(std::function<void(Element&, const float&)> cb) { onClick = std::move(cb); }

    public:
    //hierarchy
    Element* parent = nullptr;

    virtual void update(const float dt);
    virtual bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

    virtual sf::FloatRect getBounds() {return sf::FloatRect(e_position, e_size);}

    virtual void layoutPass();
    virtual void sizePass(){
        if(e_sizeType == sizeTypes::absolute) return;
    }

    virtual void drawDebug(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

    virtual ~Element() = default; 
    Element() {
        ElementCount++;
        intr_position.setDuration(0.25f);
        intr_size.setDuration(0.25f);

        intr_size = e_size;
        intr_position = e_position;
    }
};

class Container : public Element {
public:
    std::vector<std::unique_ptr<Element>> children;

    //override to pass data to children
    void update(const float dt) override {
        if(!enabled) return;
        Element::update(dt);
        passUpdate(dt);
    }

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
        if(!enabled) return false;
        for (auto& child : children)
            if(child->handleEvent(event, window)) return true;

        return Element::handleEvent(event, window);
    }

    void passUpdate(const float dt) {
        for (auto& child : children)
            child->update(dt);
    }
    void passDraw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
        for (auto& child : children)
            child->draw(target, states);
    }

    template <typename T, typename... Args>
    T* CreateElement(Args&&... args) {
        static_assert(std::is_base_of_v<Element, T>, "T must derive from Element");
        auto element = std::make_unique<T>(std::forward<Args>(args)...);
        element->parent = this;
        auto ptr = element.get();
        children.push_back(std::move(element));
        return ptr;
    }

    void layoutPass() override {
        Element::layoutPass();
        for (auto& child : children)
            child->layoutPass();
    }

    void setScheme(const ColorScheme& scheme) override {
        e_scheme = scheme;
        for (auto& child : children)
            child->setScheme(scheme);
    }

    void setDebuggingMode(bool bug) override final{ debug = bug; for (auto& child : children) child->setDebuggingMode(bug); }

};

};