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

	std::string				m_levelPath;
	std::shared_ptr<Entity>	m_player;
	PlayerConfig			m_playerConfig;
	bool					m_drawTextures;
	bool					m_drawCollision;
	bool					m_drawGrid;
	bool					m_verticalResolved;
	bool					m_horizontalResolved;
	bool					m_viewSet				= false;
	const Vec2				m_gridSize				= { 64, 64 };
	sf::Text				m_gridText;
	sf::Text				m_playText;
	int						m_score					= 0;
	int						m_swordDestroyedLast	= 0;
	Vec2					m_mPos;
	sf::CircleShape			m_mShape;
	sf::View				m_playerView;
	sf::View				m_landScapeView;


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
	void setView(sf::View&, float);
	void spawnPlayer();
	Vec2 windowToWorld(const Vec2&) const;
	Vec2 gridtoMidPixel(float, float, std::shared_ptr<Entity>);
	void spawnSword();
	void damage(std::shared_ptr<Entity>);

public:
	ScenePlay(std::shared_ptr<GameEngine>, const std::string&);
};

#endif