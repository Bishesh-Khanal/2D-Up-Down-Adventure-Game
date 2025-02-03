#pragma once

class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float, float);

	bool operator == (const Vec2&) const;
	bool operator != (const Vec2&) const;

	Vec2 operator + (const Vec2&) const;
	Vec2 operator - (const Vec2&) const;
	Vec2 operator * (float) const;
	Vec2 operator / (float) const;

	void operator += (const Vec2&);
	void operator -= (const Vec2&);
	void operator *= (float);
	void operator /= (float);

	float distq(const Vec2&) const;
	float angle(const Vec2&) const;
};