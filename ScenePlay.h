#pragma once
#ifndef SCENEPLAY_H
#define SCENEPLAY_H

#include "Scene.h"

class ScenePlay :public Scene
{
private:
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

	struct Intersect
	{
		bool result;
	};

	std::string				m_levelPath;
	std::shared_ptr<Entity>	m_player;
	PlayerConfig			m_playerConfig;
	bool					m_drawTextures			= true;
	bool					m_drawCollision			= false;
	bool					m_drawGrid				= false;
	bool					m_follow				= true;
	const Vec2				m_gridSize				= { 64, 64 };
	sf::Text				m_gridText;
	sf::Text				m_playText;
	int						m_swordDestroyedLast	= 0;
	Vec2					m_mPos;
	sf::CircleShape			m_mShape;
	sf::View				m_view;


	void init(const std::string&);

	void onEnd()							override;
	void update()							override;
	void sRender()							override;
	void sDoAction(const Action&)			override;
	const ActionMap& getActionMap() const	override;

	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sDebug();
	void sLifeSpan();

	void loadLevel(const std::string&);
	void setView();
	void spawnPlayer();
	void spawnEnemy(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, size_t, const std::string&, int, float);
	std::vector<float> parseValues(const std::string&);
	void selectColor(const std::string&, sf::Color&);
	Vec2 windowToWorld(const Vec2&) const;
	Vec2 gridtoMidPixel(float, float, std::shared_ptr<Entity>);
	void spawnSword();
	void damage(std::shared_ptr<Entity>, int , int);
	Intersect intersection(const Vec2&, const Vec2&);
	void solveCollision(std::shared_ptr<Entity>, std::shared_ptr<Entity>, const Vec2&);

public:
	ScenePlay(std::shared_ptr<GameEngine>, const std::string&);
};

#endif