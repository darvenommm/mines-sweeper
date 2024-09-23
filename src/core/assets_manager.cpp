#include <format>

#include "assets_manager.hpp"

std::unique_ptr<TexturesMap> AssetsManager::textures = std::make_unique<TexturesMap>();
std::unique_ptr<FontsMap> AssetsManager::fonts = std::make_unique<FontsMap>();

sf::Texture &AssetsManager::get_texture(std::string path)
{
  try
  {
    return textures->at(path);
  }
  catch (const std::out_of_range &)
  {
    sf::Texture texture{};

    if (!texture.loadFromFile(path))
      throw std::runtime_error(std::format("Failed to load texture from file: {}", path));

    textures->emplace(path, texture);

    return textures->at(path);
  }
}

sf::Font &AssetsManager::get_font(std::string path)
{
  try
  {
    return fonts->at(path);
  }
  catch (const std::out_of_range &)
  {
    sf::Font font{};

    if (!font.loadFromFile(path))
      throw std::runtime_error(std::format("Failed to load texture from file: {}", path));

    fonts->emplace(path, font);

    return fonts->at(path);
  }
}
