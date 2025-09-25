#pragma once

namespace myui{

struct ColorScheme{
    sf::Color background;
    sf::Color foreground;
    sf::Color accent;
    sf::Color text;
    sf::Color border;
    sf::Color hover;
    sf::Color pressed;
    sf::Color disabled;

    bool operator==(const ColorScheme& other) const {
        return foreground == other.foreground &&
               hover      == other.hover &&
               pressed    == other.pressed &&
               disabled   == other.disabled &&
               background == other.background;
    }

    bool operator!=(const ColorScheme& other) const {
        return !(*this == other);
    }
};

struct DefaultSchemes {
    static ColorScheme light() {
        return {
            sf::Color(220, 220, 225), // background
            sf::Color(200, 200, 200), // foreground
            sf::Color(0, 120, 215),   // accent
            sf::Color(0, 0, 0),       // text
            sf::Color(120, 120, 120), // border
            sf::Color(180, 180, 180), // hover
            sf::Color(150, 150, 150), // pressed
            sf::Color(160, 160, 160)  // disabled
        };
    }

    static ColorScheme dark() {
        return {
            sf::Color(30, 30, 30),    // background
            sf::Color(45, 45, 45),    // foreground
            sf::Color(0, 120, 215),   // accent
            sf::Color(255, 255, 255), // text
            sf::Color(80, 80, 80),    // border
            sf::Color(50, 50, 50),    // hover
            sf::Color(45, 45, 45),    // pressed
            sf::Color(60, 60, 60)     // disabled
        };
    }

    static ColorScheme TextureTints() {
        return {
            sf::Color::White, // background
            sf::Color::White, // foreground
            sf::Color(235, 235, 235),   // accent
            sf::Color::White,       // text
            sf::Color::White, // border
            sf::Color(210, 210, 210), // hover
            sf::Color(180, 180, 180), // pressed
            sf::Color(120, 120, 120)  // disabled
        };
    }
};

}