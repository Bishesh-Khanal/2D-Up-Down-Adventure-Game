#pragma once
#include "Entity.h"

class Physics {
public:
	static Vec2 GetOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
};