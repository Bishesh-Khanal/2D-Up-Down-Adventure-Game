#include "Physics.h"
#include <cmath>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	if (a->hasComponent<CBoundingBox>() && b->hasComponent<CBoundingBox>())
	{
		float x1 = a->getComponent<CTransform>().pos.x;
		float x2 = b->getComponent<CTransform>().pos.x;
		float y1 = a->getComponent<CTransform>().pos.y;
		float y2 = b->getComponent<CTransform>().pos.y;

		float dx = std::abs(x2 - x1);
		float overlap_x = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - dx;

		float dy = std::abs(y2 - y1);
		float overlap_y = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - dy;

		if (overlap_x > 0 && overlap_y > 0)
		{
			return Vec2(overlap_x, overlap_y);
		}
		else
		{
			return Vec2(0.0f, 0.0f);
		}
	}
	else
	{
		return Vec2(0.0f, 0.0f);
	}
}
Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	if (a->hasComponent<CBoundingBox>() && b->hasComponent<CBoundingBox>())
	{
		float x1 = a->getComponent<CTransform>().prevPos.x;
		float x2 = b->getComponent<CTransform>().prevPos.x;
		float y1 = a->getComponent<CTransform>().prevPos.y;
		float y2 = b->getComponent<CTransform>().prevPos.y;

		float dx = std::abs(x2 - x1);
		float overlap_x = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x - dx;

		float dy = std::abs(y2 - y1);
		float overlap_y = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y - dy;

		return Vec2(overlap_x, overlap_y);
	}
	else
	{
		return Vec2(0.0f, 0.0f);
	}
}
