#include "Assets.h"
#include <stdexcept>

void Assets::addTexture(const std::string& name, const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Error loading texture from: " << path << std::endl;
		return;
	}
	else
	{
		m_textures[name] = std::move(texture);
		std::cout << "Successfully loaded texture: " << name << " from: " << path << std::endl;
	}
}

void Assets::addSound(const std::string& name, const std::string& path)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path))
	{
		std::cerr << "Error loading sound from: " << path << std::endl;
		return;
	}
	else
	{
		m_sounds[name] = std::move(buffer);
		std::cout << "Successfully loaded sound: " << name << " from: " << path << std::endl;
	}
}

void Assets::addAnimations(const std::string& name, const Animation& animation)
{
	m_animations[name] = animation;
	std::cout << "Successfully loaded animation: " << name << std::endl;
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cerr << "Error loading font from: " << path << std::endl;
		return;
	}
	else
	{
		m_fonts[name] = std::move(font);
		std::cout << "Successfully loaded font: " << name << " from: " << path << std::endl;
	}
}

const sf::Texture& Assets::getTexture(const std::string& name)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
		return it->second;
	throw std::runtime_error("Texture not found: " + name);

}

const sf::SoundBuffer& Assets::getSound(const std::string& name)
{
	auto it = m_sounds.find(name);
	if (it != m_sounds.end())
		return it->second;
	throw std::runtime_error("Sound not found: " + name);

}

const Animation& Assets::getAnimation(const std::string& name)
{
	auto it = m_animations.find(name);
	if (it != m_animations.end())
		return it->second;
	throw std::runtime_error("Animation not found: " + name);

}

const sf::Font& Assets::getFont(const std::string& name) {
	auto it = m_fonts.find(name);
	if (it == m_fonts.end()) {
		throw std::runtime_error("Font '" + name + "' not found in assets.");
	}
	return it->second;
}

void Assets::debugTextures() const {
	std::cout << "Currently loaded textures:" << std::endl;
	for (const auto& [name, texture] : m_textures) {
		std::cout << " - " << name << std::endl;
	}
}

void Assets::debugSounds() const {
	std::cout << "Currently loaded sounds:" << std::endl;
	for (const auto& [name, sound] : m_sounds) {
		std::cout << " - " << name << std::endl;
	}
}

void Assets::debugAnimations() const {
	std::cout << "Currently loaded animations:" << std::endl;
	for (const auto& [name, animation] : m_animations) {
		std::cout << " - " << name << std::endl;
	}
}

void Assets::debugFonts() const {
	std::cout << "Currently loaded fonts:" << std::endl;
	for (const auto& [name, font] : m_fonts) {
		std::cout << " - " << name << std::endl;
	}
}


