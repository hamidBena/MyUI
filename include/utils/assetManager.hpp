#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

namespace myui{
class AssetManager {
public:
    static AssetManager& get();

    sf::Texture& getTexture(const std::string& filename);
    sf::Font& getFont(const std::string& filename);

    static void setAssetDirectory(const fs::path& dir);
    static sf::Font& getDefaultFont(); // 🔹 new convenience

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

private:
    AssetManager();

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

    static fs::path asset_dir;
};
}
