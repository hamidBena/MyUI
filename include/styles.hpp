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
            sf::Color(220, 220, 225, 200), // background
            sf::Color(200, 200, 200, 200), // foreground
            sf::Color(0, 120, 215, 200),   // accent
            sf::Color(0, 0, 0),       // text
            sf::Color(120, 120, 120), // border
            sf::Color(180, 180, 180, 230), // hover
            sf::Color(150, 150, 150), // pressed
            sf::Color(160, 160, 160)  // disabled
        };
    }

    static ColorScheme dark() {
        return {
            sf::Color(30, 30, 30, 200),    // background
            sf::Color(45, 45, 45, 200),    // foreground
            sf::Color(0, 120, 215, 200),   // accent
            sf::Color(255, 255, 255), // text
            sf::Color(80, 80, 80),    // border
            sf::Color(50, 50, 50, 230),    // hover
            sf::Color(45, 45, 45),    // pressed
            sf::Color(60, 60, 60)     // disabled
        };
    }

    static ColorScheme Rose() {
        return {
            sf::Color(255, 240, 245, 200), // background (lavender blush)
            sf::Color(255, 228, 225, 200), // foreground (misty rose)
            sf::Color(176, 224, 230, 200), // accent (powder blue)
            sf::Color(75, 0, 110),    // text (indigo)
            sf::Color(255, 182, 193), // border (pink)
            sf::Color(255, 222, 173, 230), // hover (navajo white)
            sf::Color(221, 160, 221), // pressed (plum)
            sf::Color(200, 200, 200)  // disabled
        };
    }

    static ColorScheme Blue() {
        return {
            sf::Color(0, 43, 54, 200),     // background
            sf::Color(7, 54, 66, 200),     // foreground
            sf::Color(38, 139, 210, 200),  // accent
            sf::Color(211, 228, 230), // text
            sf::Color(88, 110, 117),  // border
            sf::Color(0, 60, 70),     // hover
            sf::Color(20, 80, 100),   // pressed
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