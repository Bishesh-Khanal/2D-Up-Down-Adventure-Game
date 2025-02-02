#pragma once
#include "Vec2.h"
#include "Animation.h"

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class CDraggable
{
public:
	bool has = false;
	bool dragging = false;

	CDraggable()
	{
	}

	CDraggable(bool h)
		: has(h)
	{
	}
};

class CTransform
{
public:
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 prevPos = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	Vec2 scale = { 1.0f, 1.0f };
	float rotation = 0.0f;
	float speed = 0.0f;
	float angle = 1.0f;
	bool has = false;

	CTransform()
	{
	}

	CTransform(const Vec2& p)
		: pos(p)
	{
	}

	CTransform(const Vec2& p, const Vec2& v, float r = 0, float s = 0, float a = 0)
		: pos(p)
		, velocity(v)
		, rotation(r)
		, speed(s)
		, angle(a)
	{
	}
};

class CInput
{
public:
	bool has = false;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool cancut = true;

	CInput()
	{
	}
};

class CCollision
{
public:
	bool has = false;

	float radius = 0.0f;

	CCollision()
	{
	}

	CCollision(float r)
		: radius(r)
	{
	}
};

class CScore
{
public:
	bool has = false;

	float score = 0.0f;

	CScore()
	{
	}

	CScore(float s)
		: score(s)
	{
	}
};

class CLifespan
{
public:
	bool has = false;

	float total = 0.0f;
	float remaining = 0.0f;

	CLifespan()
	{
	}

	CLifespan(float span, bool h = true)
		: total(span*60.0f)
		, remaining(span*60.0f)
		, has(h)
	{
	}
};

class CBoundingBox
{
public:
	bool has = false;

	Vec2 boundingbox = { 0.0f, 0.0f };
	Vec2 halfSize = { 0.0f, 0.0f };
	sf::Color boxColor;
	sf::RectangleShape rectangle;

	CBoundingBox()
	{
	}

	CBoundingBox(const Vec2& bbox, const sf::Color& color = sf::Color::White, bool h = true)
		: boundingbox(bbox)
		, boxColor(color)
		, has(h)
		, halfSize(bbox / 2)
	{
		rectangle.setSize(sf::Vector2f(bbox.x, bbox.y));
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color(boxColor));
		rectangle.setOutlineThickness(1.0f);
		rectangle.setOrigin(bbox.x / 2, bbox.y / 2);
	}
};

class CAnimation
{
public:
	bool has = false;
	bool destroy = false;

	Animation animation;

	CAnimation()
	{
	}

	CAnimation(const Animation anim, bool d, bool h = true)
		: animation(anim)
		, destroy(d)
		, has(h)
	{
	}
};

class CState
{
public:
	std::string state = "StandSide";
	bool has = false;

	CState()
	{
	}

	CState(const std::string& s, bool h = true)
		: state(s)
		, has(h)
	{
	}
};