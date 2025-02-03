#pragma once
#include "Components.h"

#include <tuple>
#include <string>
#include <memory>

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifespan,
	CInput,
	CBoundingBox,
	CAnimation,
	CState,
	CHealthBar,
	CPatrol,
	CChase,
	CDamage
> ComponentTuple;

class Entity
{
private:
	size_t			m_id = 0;
	std::string		m_tag = "default";
	bool			m_active = true;
	ComponentTuple	m_components;

	Entity(size_t, const std::string&);

	friend class EntityManager;

public:
	Entity();
	/*
	std::shared_ptr<CTransform>	cTransform;
	std::shared_ptr<CShape>		cShape;
	std::shared_ptr<CInput>		cInput;
	std::shared_ptr<CCollision>	cCollision;
	std::shared_ptr<CScore>		cScore;
	std::shared_ptr<CLifespan>	cLifespan;
	*/

	void destroy();
	const bool& isActive() const;
	const std::string& tag() const;
	const size_t& id() const;

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}
};