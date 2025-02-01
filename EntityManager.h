#pragma once
#include <vector>
#include <map>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
private:
	EntityVec m_entities;
	EntityVec m_toadd;
	EntityMap m_entityMap;
	size_t totalentities = 0;

public:
	EntityManager();
	void update();
	std::shared_ptr<Entity> addEntity(const std::string&);
	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string&);
};