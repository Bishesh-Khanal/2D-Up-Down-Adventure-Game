#pragma once

#include "SFML/Graphics.hpp"
#include "Vec2.h"

#include <iostream>
#include <string>

class Animation
{
private:
	friend class ScenePlay;

	std::string m_name = "DEFAULT";
	size_t m_frameCount = 0;
	size_t m_gameFrame = 0;
	size_t m_animFrame = 0;
	Vec2 m_size = { 0.0f, 0.0f };
	size_t m_animationSpeed = 0;

	sf::IntRect m_rectangle;
	sf::Sprite m_sprite;
public:
	Animation();
	Animation(const std::string&, const sf::Texture&);
	Animation(const std::string&, const sf::Texture&, size_t, float);
	const std::string& getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
	void update();
	bool hasEnded() const;
};