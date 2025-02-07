#pragma once
#include "Assets.h"

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> Scenes;

class GameEngine
{
private:
	friend class SceneMenu;
	friend class ScenePlay;

	Scenes					m_scenes;
	sf::RenderWindow		m_window;
	Assets					m_assets;
	std::string				m_currentScene;
	size_t					m_simulationSpeed	= 1;
	bool					m_running			= true;
	float					m_widthW			= 1472.0f;
	float					m_heightW			= 1088.0f;
	float					m_worldWidth		= 2 * m_widthW;
	float					m_worldHeight		= 2 * m_heightW;

	void init(const std::string&);
	const std::shared_ptr<Scene>& currentScene();
	void update();

	Assets& getAssets();
	void changeScene(const std::string&, std::shared_ptr<Scene>);

	void sUserInput();

public:
	GameEngine();
	void run(std::shared_ptr<GameEngine>);
	void quit();
};