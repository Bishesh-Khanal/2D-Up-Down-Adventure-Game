#include "Animation.h"
#include <cmath>

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& texture)
	: Animation(name, texture, 1, 0.0f)
{
}

Animation::Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, float animationSpeed)
	: m_name(name)
	, m_sprite(texture)
	, m_frameCount(frameCount)
	, m_gameFrame(0)
	, m_animationSpeed(animationSpeed)
{
	m_size = Vec2(texture.getSize().x / m_frameCount, texture.getSize().y);
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
}

void Animation::update()
{
	if (m_animationSpeed != 0 && m_frameCount > 0)
	{
		m_gameFrame++;
		m_animFrame = (m_gameFrame / m_animationSpeed) % m_frameCount;
		m_rectangle = sf::IntRect(static_cast<int>(std::floor(m_animFrame) * m_size.x), 0, m_size.x, m_size.y);
		m_sprite.setTextureRect(m_rectangle);
	}
}


const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

bool Animation::hasEnded() const
{
	if (m_animFrame >= m_frameCount - 1)
	{
		return true;
	}
	return false;
}


