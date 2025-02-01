#pragma once
#include <string>
#include "Vec2.h"

class Action
{
private:
	std::string m_name = "NONE";
	std::string m_type = "NONE";
	Vec2 m_mousePos = { 0.0f, 0.0f };

public:
	Action();
	Action(const std::string&, const std::string&);
	Action(const std::string&, const std::string&, const Vec2&);

	const std::string& name() const;
	const std::string& type() const;
	const Vec2& pos() const;
	std::string toString() const;
};