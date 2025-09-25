#include "utils/assetManager.hpp"
#include "utils/env.hpp"
#include <iostream>

namespace myui{

fs::path AssetManager::asset_dir = [] {
    fs::path current = env::exe_dir();
    while (!current.empty()) {
        fs::path try_path = current / "assets";
        if (fs::exists(try_path) && fs::is_directory(try_path)) {
            std::cout << try_path << '\n';
            return try_path;
        }
        current = current.parent_path();
    }
    throw std::runtime_error("Could not locate 'assets/' directory.");
}();

AssetManager& AssetManager::get() {
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager() {}

void AssetManager::setAssetDirectory(const fs::path& dir) {
    asset_dir = dir;
}

sf::Texture& AssetManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) return *it->second;

    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(asset_dir / filename))
        throw std::runtime_error("Failed to load texture: " + filename);

    auto& ref = *tex;
    textures[filename] = std::move(tex);
    return ref;
}

sf::Font& AssetManager::getFont(const std::string& filename) {
    auto it = fonts.find(filename);
    if (it != fonts.end()) return *it->second;

    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile((asset_dir / filename).string()))
        throw std::runtime_error("Failed to load font: " + filename);

    auto& ref = *font;
    fonts[filename] = std::move(font);
    return ref;
}

sf::Font& AssetManager::getDefaultFont() {
    // Always try to load a bundled font
    return AssetManager::get().getFont("default.ttf");
}

}