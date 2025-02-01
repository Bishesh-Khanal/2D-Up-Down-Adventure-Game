#pragma once

#include <map>
#include "Animation.h"
#include "SFML/Audio.hpp"

class Assets {
public:
    void addTexture(const std::string&, const std::string&);
    void addAnimations(const std::string&, const Animation&);
    void addFont(const std::string&, const std::string&);
    void addSound(const std::string&, const std::string&);

    const sf::Texture& getTexture(const std::string&);
    const Animation& getAnimation(const std::string&);
    const sf::Font& getFont(const std::string&);
    const sf::SoundBuffer& getSound(const std::string&);

    void debugTextures() const;
    void debugAnimations() const;
    void debugFonts() const;
    void debugSounds() const;

private:
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation> m_animations;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_sounds;
};
