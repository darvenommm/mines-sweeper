#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include <memory>
#include <SFML/Graphics.hpp>

using TexturesMap = std::map<std::string, sf::Texture>;
using FontsMap = std::map<std::string, sf::Font>;

class AssetsManager
{
public:
  static sf::Texture &get_texture(std::string);
  static sf::Font &get_font(std::string);

private:
  static std::unique_ptr<TexturesMap> textures;
  static std::unique_ptr<FontsMap> fonts;
};

#endif
