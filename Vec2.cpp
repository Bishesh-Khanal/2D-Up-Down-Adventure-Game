#include "Vec2.h"

#include <cmath>

Vec2::Vec2()
{
}

Vec2::Vec2(float x, float y)
	: x(x)
	, y(y)
{
}

bool Vec2::operator == (const Vec2& rhs) const
{
	return(x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return(x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2:: operator * (float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2:: operator / (float val) const
{
	return Vec2(x / val, y / val);
}

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator *= (float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator /= (float val)
{
	x /= val;
	y /= val;
}

float Vec2::distq(const Vec2& rhs) const
{
	return((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}

float Vec2::angle(const Vec2& rhs) const
{
	return(atan2f(y - rhs.y, x - rhs.x));
}